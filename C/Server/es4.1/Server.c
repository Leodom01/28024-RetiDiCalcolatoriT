#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>

#define LINE_LENGTH 254
#define BUFFER_SIZE 4096

// In caso in cui il main thread generi dei figli, poi ricordati di fare signal(gestore, SIGCHLD)
void gestore(int signo)
{
    int stato;
    printf("In esecuzione gestore di SIGCHLD\n");
    wait(&stato);
    printf("Stato figlio: %d\n", stato >> 8);
}

int main(int argc, char **argv)
{

    int sock, open_conn_sock, udp_fd, fd_dir, nready, maxfdnum;
    char filename[FILENAME_MAX + 1], dirname[FILENAME_MAX + 1], worldToDel[FILENAME_MAX + 1];
    char buffer[BUFFER_SIZE];
    fd_set readset;
    int len, nread, nwrite, ris, port;
    const int on = 1;
    struct sockaddr_in clientaddr, servaddr;
    struct hostent *host;

    if (argc != 2)
    {
        printf("Numero di parametri sbagliati...\n");
        exit(1);
    }

    port = atoi(argv[1]);
    if (port == 0 || port < 1024 || port > 65535)
    {
        printf("Fornisci una porta valida...\n");
        exit(1);
    }
    printf("In ascolto su %d", port);

    memset((char *)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(2);
    }
    printf("Socket TCP creata\n");

    udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_fd < 0)
    {
        perror("socket");
        exit(2);
    }
    printf("Socket UDP creata\n");

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
        perror("etsockopt");
        exit(2);
    }
    printf("Socket opt TCP settate\n");

    if (setsockopt(udp_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
        perror("etsockopt");
        exit(2);
    }
    printf("Socket opt UDP settate\n");

    if (bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind");
        exit(2);
    }
    printf("Bind TCP successful\n");

    if (bind(udp_fd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind");
        exit(2);
    }
    printf("Bind UDP successful\n");

    if (listen(sock, 5) < 0)
    {
        perror("listen");
        exit(2);
    }
    printf("Coda d'ascolto creata\n");

    signal(SIGCHLD, gestore);

    while (1)
    {
        FD_SET(sock, &readset);
        FD_SET(udp_fd, &readset);
        maxfdnum = (udp_fd > sock) ? udp_fd + 1 : sock + 1;
        printf("In attesa di select...\n");
        printf("Con maxfd: %d\n", maxfdnum);
        if ((nready = select(maxfdnum, &readset, NULL, NULL, NULL)) < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            else
            {
                perror("select");
                exit(3);
            }
        }
        if (FD_ISSET(udp_fd, &readset))
        {
            // Richiesta udp
            printf("Richiesta UDP arrivata\n");
            len = sizeof(clientaddr);
            if (recvfrom(udp_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientaddr, &len) < 0)
            {
                perror("recvfrom");
                continue;
            }
            printf("Ricevuta stringa: %s\n", buffer);

            char *strToken = strtok(buffer, " ");
            strcpy(filename, strToken);
            strToken = strtok(NULL, " ");
            strcpy(worldToDel, strToken);
            printf("Ricevuto file e parola: %s %s\n", filename, worldToDel);

            // PARTE LOGICA UDP
            char tempFilename[FILENAME_MAX + 1];
            strcpy(tempFilename, "temp_");
            strcat(tempFilename, filename);
            printf("Il file di supporto sarà %s\n", tempFilename);
            int fd_support = open(tempFilename, O_WRONLY | O_CREAT, 0777);
            int fd_current = open(filename, O_RDONLY);

            buffer[0] = '\0';
            char tempChar;
            int matchCount = 0;
            while (read(fd_current, &tempChar, 1) > 0)
            {
                int buffLen = strlen(buffer);
                buffer[buffLen] = tempChar;
                buffer[buffLen + 1] = '\0';
                buffLen++;
                if (buffLen == strlen(worldToDel))
                {
                    if (strcmp(buffer, worldToDel) == 0)
                    {
                        matchCount++;
                        buffer[0] = '\0';
                    }
                    else
                    {
                        write(fd_support, &buffer[0], 1);
                        for (int i = 0; i < strlen(buffer); i++)
                        {
                            buffer[i] = buffer[i + 1];
                        }
                    }
                }
            }
            write(fd_support, buffer, strlen(buffer));
            close(fd_current);
            close(fd_support);
            rename(tempFilename, filename);
            // FINE PARTE LOGICA UDP--------------------

            if (sendto(udp_fd, &matchCount, sizeof(int), 0, (struct sockaddr *)&clientaddr, len) < 0)
            {
                perror("sendto");
                continue;
            }
            printf("Risultato %d inviato!\n", matchCount);
            printf("Risolta richiesta UDP\n");
        }
        if (FD_ISSET(sock, &readset))
        {
            // Richiesta TCP
            printf("Richiesta TCP arrivata\n");
            if ((open_conn_sock = accept(sock, (struct sockaddr *)&clientaddr, &len)) < 0)
            {
                if (errno == EINTR)
                {
                    continue;
                }
                else
                {
                    perror("accept");
                    continue;
                }
            }
            if (fork() == 0)
            {
                printf("FIGLIO: figlio avviato\n");
                close(sock);
                len = sizeof(clientaddr);
                while (read(open_conn_sock, &dirname, sizeof(dirname)) > 0)
                {
                    DIR *dir;
                    DIR *innerdir;
                    struct dirent *dd;
                    struct dirent *innerdirent;
                    char innerName[FILENAME_MAX * 2 + 1];
                    int counter = 0;
                    dir = opendir(dirname);
                    if (dir == NULL)
                    {
                        char res[FILENAME_MAX] = "Non trovata";
                        write(open_conn_sock, res, strlen(res)+1);
                    }
                    else
                    {
                        while ((dd = readdir(dir)) != NULL)
                        {
                            if (dd->d_type == DT_DIR && strcmp(dd->d_name, ".") != 0 && strcmp(dd->d_name, "..") != 0)
                            {
                                strcpy(innerName, dirname);
                                strcat(innerName, "/");
                                strcat(innerName, dd->d_name);
                                innerdir = opendir(innerName);
                                while ((innerdirent = readdir(innerdir)) != NULL)
                                {
                                    char toSend[4096];
                                    strcpy(toSend, innerdirent->d_name);
                                    strcat(toSend, "\n");
                                    write(open_conn_sock, toSend, strlen(toSend));
                                    printf("Trovato file: %s\n", innerdirent->d_name);
                                }
                                closedir(innerdir);
                            }
                        }
                        char binZero = 0;
                        write(open_conn_sock, &binZero, 1);
                        closedir(dir);
                    }
                }
                close(open_conn_sock);
                printf("FIGLIO: Connessione terminata...\n");
            }
            close(open_conn_sock);
        }
    }
}
