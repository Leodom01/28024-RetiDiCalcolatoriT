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

#define LINE_LENGTH 256

typedef struct
{
    int op1;
    int op2;
    char operazione;
} Request;

// Chiamta attesa: ./Client <hostname> <port>

int main(int argc, char *argv[])
{

    struct hostent *host;
    struct sockaddr_in clientaddr, servaddr;
    int port, sock, num1, num2, len;
    char res[LINE_LENGTH];
    char okstr[LINE_LENGTH];
    char c;
    int ok;
    Request req;

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
    printf("Inserisci il primo operando:\n");
    while ((ok = scanf("%i", &num1)) != EOF)
    {
        if (ok != 1)
        {
            // Rischio di bloccare la testina e devo fare questo pastrocchio per sistemarlo
            do
            {
                c = getchar();
                printf("%c", c);
            } while (c != '\n');
            printf("Inserisci il primo operando:\n");
            continue;
        }

        req.op1 = num1;
        // Consumo quello che resta
        gets(okstr);

        printf("Inserisci il secondo operando:\n");
        while ((ok = scanf("%i", &num2)) != 1)
        {
            do
            {
                c = getchar();
                printf("%c", c);
            } while (c != '\n');
            printf("Inserisci il secondo operando:\n");
            continue;
        }

        req.op2 = num2;
        // Consumo quello che resta
        gets(okstr);

        do
        {
            printf("Inserisci l'operazione: + per somma, - per sottr, * per molt, / per divisione\n");
            c = getchar();
            gets(okstr);
        } while (c != '+' && c != '-' && c != '*' && c != '/');

        req.operazione = c;

        printf("Recap operazione richiesta: %i %c %i\n", req.op1, req.operazione, req.op2);

        // Invio richiesta
        len = sizeof(servaddr);
        if (sendto(sock, &req, sizeof(Request), 0, (struct sockaddr *)&servaddr, len) < 0)
        {
            perror("sendto");
            continue;
        }

        // Ricevo risultato
        printf("Attendo il risultato...\n");
        if (recvfrom(sock, res, sizeof(char[LINE_LENGTH]), 0, (struct sockaddr *)&servaddr, &len) < 0)
        {
            perror("recvfrom");
            continue;
        }

        printf("Risultato di tutta sta pugnetta: %s\n", res);

        printf("Inserisci il primo operando o EOF per finire:");
    }

    close(sock);
    printf("Termino questo client");
    exit(0);
}