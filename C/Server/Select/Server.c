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

int contaFile(char dirname[])
{
    DIR *dir;
    struct dirent *dd;
    int counter = 0;
    dir = opendir(dirname);
    if(dir == NULL){
        printf("Apro dir nulla\n");
        return -1;
    }
    while ((dd = readdir(dir)) != NULL)
    {
        printf("Trovato file %s\n", dd->d_name);
        counter++;
    }
    printf("Trovati %d files\n", counter);
    closedir(dir);
    return counter;
}

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

    int sock, open_conn_sock, udp_fd, fd_file, nready, maxfdnum;
    char buffer[BUFFER_SIZE];
    char filename[FILENAME_MAX + 1], dirname[FILENAME_MAX + 1];
    fd_set readset;
    int len, nread, nwrite, num, ris, port;
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
        maxfdnum = (udp_fd > sock)? udp_fd+1:sock+1;
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
            if ((nread = recvfrom(udp_fd, dirname, sizeof(char[FILENAME_MAX + 1]), 0, (struct sockaddr *)&clientaddr, &len)) < 0)
            {
                perror("recvfrom");
                continue;
            }
            num = contaFile(dirname);
            if (sendto(udp_fd, &num, sizeof(int), 0, (struct sockaddr *)&clientaddr, len) < 0)
            {
                perror("sendto");
                continue;
            }
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
                close(sock);
                if (read(open_conn_sock, filename, sizeof(char[FILENAME_MAX + 1])) <= 0)
                {
                    perror("read");
                    exit(4);
                }
                printf("Tento di aprire %s\n", filename);
                fd_file = open(filename, O_RDONLY | O_CREAT, 0644);
                if (fd_file < 0)
                {
                    write(open_conn_sock, "N", 1);
                }
                else
                {
                    printf("Appena aperto file %s con fd %d\n", filename, fd_file);
                    printf("FIGLIO: Scrivo file\n");
                    while ((nread = read(fd_file, buffer, sizeof(buffer))) > 0)
                    {
                        if ((nwrite = write(open_conn_sock, buffer, nread)) < 0)
                        {
                            perror("write");
                            exit(4);
                        }
                        printf("Scritto %s\n", buffer);
                    }
                    close(fd_file);
                }
                close(open_conn_sock);
                exit(0);
            }
            close(open_conn_sock);
        }
    }
}

