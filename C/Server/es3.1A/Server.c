#include <dirent.h>
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

#define LINE_LENGTH 254

// Chiamta attesa: ./Server <porta da esporre>

int main(int argc, char **argv)
{

    int sock, port, len, res, file_fd, currentLen;
    char readChar;
    char filename[FILENAME_MAX + 1];
    struct sockaddr_in servaddr, clientaddr;
    struct hostent *clienthost;

    if (argc != 2)
    {
        printf("Error: %s serverPort\n", argv[0]);
        exit(1);
    }

    port = atoi(argv[1]);
    if (port == 0)
    {
        printf("Error: %s serverAddress serverPort\n", argv[0]);
        exit(1);
    }
    else if (port < 1024 || port > 65535)
    {
        printf("Error:la porta deve essere fra 1024 e 65535t\n");
        exit(1);
    }

    // Init servaddr
    memset((char *)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);

    // Creazione e bind socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(2);
    }
    if (bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("Errore in fase bind socket");
        exit(2);
    }
    printf("Socket creata e connessa");

    // Ciclo ricezione del server
    while (1)
    {
        len = sizeof(struct sockaddr_in);
        if (recvfrom(sock, filename, sizeof(char[FILENAME_MAX + 1]), 0, (struct sockaddr *)&clientaddr, &len) < 0)
        {
            perror("Errore in fase di ricezione");
            continue;
        }
        if (fork() == 0)
        {
            printf("Sono il figlio e ora mi do da fare\n");
            // Dentro al figlio schiavo
            printf("File richiesto: %s\n", filename);

            clienthost = gethostbyaddr((char *)&clientaddr.sin_addr, sizeof(clientaddr.sin_addr), AF_INET);
            if (clienthost == NULL)
            {
                perror("Errore in fase di lettura del client");
                exit(1);
            }
            printf("Operazione richiesta da %s %i\n", clienthost->h_name, (unsigned)ntohs(clientaddr.sin_port));

            if ((file_fd = open(filename, O_RDONLY)) < 0)
            {
                perror("open");
                exit(1);
            }

            res = 0;
            currentLen = 0;
            // Inefficient, would be bettter to read the whole string and then parse
            while (read(file_fd, &readChar, sizeof(char)) > 0)
            {
                if (readChar == ' ' || readChar == '\n')
                {
                    if (currentLen > res)
                    {
                        res = currentLen;
                    }
                    currentLen = 0;
                }
                else
                {
                    currentLen++;
                }
            }
            // Nel caso in cui l'ultima parola sia la più lunga
            if (currentLen > res)
            {
                res = currentLen;
            }
            currentLen = 0;

            if (close(file_fd) < 0)
            {
                perror("close");
                exit(1);
            }

            len = sizeof(clientaddr);
            if (sendto(sock, &res, sizeof(res), 0, (struct sockaddr *)&clientaddr, len) < 0)
            {
                perror("Errore in fase di sento");
                exit(1);
            }

            printf("Inviato risultato al client, attendo il prossimo...\n");
            exit(0);
        }else{
            printf("Come padre continuo");
        }
    }
}
