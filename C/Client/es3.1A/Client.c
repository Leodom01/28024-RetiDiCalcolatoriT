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

//Chiamta attesa: ./Client <hostname> <serverPort>

int main(int argc, char **argv) {
    
    int sock, result, len, port;
    struct hostent* host;
    struct sockaddr_in clientaddr, servaddr;
    char filename[FILENAME_MAX+1];

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

    //Init di clientaddr
    memset((char *)&clientaddr, 0, sizeof(struct sockaddr_in));
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_addr.s_addr = INADDR_ANY;
    clientaddr.sin_port = 0;

    //Init di servaddr
    memset((char*)&clientaddr, 0, sizeof(struct sockaddr_in));
    host = gethostbyname(argv[1]);
    if(host ==NULL){
        printf("Host not found\n");
        exit(1);
    }

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = ((struct in_addr*)(host->h_addr))->s_addr;  
    servaddr.sin_port = htons(port);

    //Socket creation
     // socket creation
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
    {
        perror("Errore in fase apertura socket.");
        exit(2);
    }

    printf("Conessoone setuppata inizio parte logica...\n");
    printf("Inserisci il nome file:\n");
    while(gets(filename)){
        len = sizeof(servaddr);
        if(sendto(sock, filename, sizeof(char[FILENAME_MAX+1]), 0, (struct sockaddr*)&servaddr, len) < 0){
            perror("sendto");
            continue;
        }

        //Ricevo il risultato
        if(recvfrom(sock, &result, sizeof(result), 0, (struct sockaddr*)&servaddr, &len) < 0){
            perror("recvfrom");
            continue;
        }

        printf("Il massimo numero nel file e` lungo %d caratteri\n", result);
        printf("Inserisci il nome file:\n");
    }
    printf("Inserisci il nome file:\n");
    close(sock);
    exit(0);
}

