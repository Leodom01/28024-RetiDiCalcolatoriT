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

//Chiama come: ./Client <host> <port>

int main(int argc, char **argv) {

    struct hostent *host;
    struct sockaddr_in clientaddr, servaddr;
    int port, sock;
    int res;
    char wordToDel[LINE_LENGTH];
    char fileName[FILENAME_MAX+1];
    char requestStr[LINE_LENGTH+FILENAME_MAX+1];
    int ok;
    int len;

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
    printf("Inserisci il nome file:\n");
    while (gets(fileName))
    {
        printf("Inserisci la parola da eliminare:\n");
        gets(wordToDel);

        printf("FILE: %s, PAROLA: %s\n", fileName, wordToDel);
        sprintf(requestStr, "%s %s", fileName, wordToDel);
        printf("Invio richiesta: %s\n", requestStr);
        len = sizeof(servaddr);
        if(sendto(sock, requestStr, sizeof(requestStr), 0, (struct sockaddr*)&servaddr, len) < 0){
            perror("sendto");
            continue;
        }
        printf("Attendo risultato...\n");
        if(recvfrom(sock, &res, sizeof(res), 0, (struct sockaddr*)&servaddr, &len) < 0){
            perror("recvfrom");
            continue;
        }
        printf("RISULTATO: %d\n", res);

        printf("Inserisci il nome file, EOF per terminare:\n");
    }

    close(sock);
    printf("Termino questo client");
    exit(0);

}

