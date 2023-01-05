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

// Chiama con ./Client <host> <serverPort>

int main(int argc, char **argv)
{
    int sock, nread, port;
    char buffer[LINE_LENGTH];
    char dir[FILENAME_MAX + 1];
    char justRead;
    struct hostent *host;
    struct sockaddr_in servaddr;

    if (argc != 3)
    {
        printf("Errore nel numero di argomenti!\n");
        exit(1);
    }

    port = atoi(argv[2]);
    if (port == 0 || port < 1024 || port > 65535)
    {
        printf("Errore, inserisci una porta giusta.");
        exit(1);
    }

    host = gethostbyname(argv[1]);
    if (host == NULL)
    {
        perror("gethostbyname");
        exit(2);
    }
    printf("Host info, name: %s, ip: %s\n", host->h_name, host->h_addr_list[1]);

    memset((char *)&servaddr, 0, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr;
    servaddr.sin_port = htons(port);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket");
        exit(2);
    }

    if (connect(sock, (struct sockaddr *)&servaddr, sizeof(struct sockaddr)) < 0)
    {
        perror("connect");
        exit(2);
    }

    printf("Inserisci il nome della directory remota:\n");
    while (gets(dir))
    {
        printf("Richiedo dir: %s\n", dir);
        if(write(sock, dir, strlen(dir)+1) < 0){
            perror("write");
            continue;
        }

        printf("Attendo risposta...\n");
        while(read(sock, &justRead, sizeof(char)) > 0){
            if(justRead == '\n'){
                printf("\n");
            }else if(justRead == 0){
                break;
            }else{
                printf("%c", justRead);
            }
        }
        printf("\n");
        printf("Finito di leggere i file...\n");

        printf("Inserisci il nome della directory remota EOF per terminare:\n");
    }
    close(sock);
    exit(0);
}


