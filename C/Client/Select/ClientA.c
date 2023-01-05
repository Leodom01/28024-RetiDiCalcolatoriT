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

//Client invocato con : ./Client <nome host> <porta>

int main(int argc, char **argv) {
    
    struct hostent *host;
    struct sockaddr_in clientaddr, servaddr;
    int port, sock, len, res;
    char dir_to_search[FILENAME_MAX+1];

    if (argc != 3)
    {
        printf("Error: %s serverAddress serverPort\n", argv[0]);
        exit(1);
    }

    port = atoi(argv[2]);
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

    // Init di clientaddr
    memset((char *)&clientaddr, 0, sizeof(struct sockaddr_in));
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_addr.s_addr = INADDR_ANY;
    clientaddr.sin_port = 0;

    // Init di serveraddr
    servaddr.sin_family = AF_INET;
    host = gethostbyname(argv[1]);

    if (host == NULL)
    {
        printf("Host not found...");
        exit(2);
    }

    // Setup server data
    servaddr.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr;
    servaddr.sin_port = htons(port);

    // socket creation
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("Errore in fase apertura socket.");
        exit(3);
    }

    // Bind di socket con server
    if (bind(sock, (struct sockaddr *)&clientaddr, sizeof(clientaddr)) < 0)
    {
        perror("Errore in fase bind.");
        exit(3);
    }

    // Socket connessa inizio la parte di logica
    printf("Inserisci il nome della directory:\n");
    while(gets(dir_to_search)){
        printf("Ricerco directory: %s\n", dir_to_search);

        len = sizeof(servaddr);
        if(sendto(sock, dir_to_search, strlen(dir_to_search)+1, 0, (struct sockaddr*)&servaddr, len) < 0){
            perror("sendto");
            printf("Inserisci il nome della directory:\n");
            continue;
        }

        printf("Attendo il risultato...\n");
        if(recvfrom(sock, &res, sizeof(res), 0, (struct sockaddr*)&servaddr, &len) < 0){
            perror("recvfrom");
            printf("Inserisci il nome della directory:\n");
            continue;
        }

        printf("Ecco il risultato: %d\n", res);


        printf("Inserisci il nome della directory, EOF per terminare:\n");
    }

    shutdown(sock, SHUT_RDWR);
    exit(0);

}

