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

// Chiamta attesa: ./Server <port>

int main(int argc, char *argv[])
{

    int sock, port, len, num1, num2, resVal;
    char result[LINE_LENGTH];
    Request req;
    char op;
    struct sockaddr_in clientaddr, servaddr;
    struct hostent *clienthost;

    // Controlla numero parametri
    if (argc != 2)
    {
        printf("%s\n", "Errore nei parametri inseriti, chiama con parametri: <porta da esporre>");
        return EXIT_FAILURE;
    }

    // Check port correctness and fill port
    int nread = 0;
    while (argv[1][nread] != '\0')
    {
        if ((argv[1][nread] < '0') || (argv[1][nread] > '9'))
        {
            printf("%s\n", "Errore nella porta passata come parametro");
            printf("Error: %s port\n", argv[1]);
            return EXIT_FAILURE;
        }
        nread++;
    }
    port = atoi(argv[1]);
    if (port < 1024 || port > 65535)
    {
        printf("%s\n", "Errore nella porta passata come parametro");
        printf("Error: %s port\n", argv[1]);
        return EXIT_FAILURE;
    }

    printf("Porta correttamente letta come: %d", port);

    memset((char *)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(port);
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0){
        perror("socket");
        exit(1);
    }
    if(bind(sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0){
        perror("Errore in fase bind socket");
        exit(1);
    }
    printf("Socket creata e connessa");

    while(1){
        //Ciclo di ricezione server
        len = sizeof(struct sockaddr_in);
        if(recvfrom(sock, &req, sizeof(Request), 0, (struct sockaddr*)&clientaddr, &len) < 0){
            perror("Errore in fase di ricezione");
            continue;
        }
        
        printf("Operazione richiesta: %d %c %d\n", req.op1, req.operazione, req.op2);
        clienthost = gethostbyaddr((char*) &clientaddr.sin_addr, sizeof(clientaddr.sin_addr), AF_INET);
        if(clienthost == NULL){
            perror("Errore in fase di lettura del client");
            continue;
        }
        printf("Operazione richiesta da %s %i\n", clienthost->h_name, (unsigned)ntohs(clientaddr.sin_port));

        if(req.operazione == '+'){
            resVal = req.op1+req.op2;
            sprintf(result, "%d", resVal);
        }else if(req.operazione == '-'){
            resVal = req.op1-req.op2;
            sprintf(result, "%d", resVal);
        } else if(req.operazione == '*'){
            resVal = req.op1*req.op2;
            sprintf(result, "%d", resVal);
        }else if(req.operazione == '/' && req.op2 != 0){
            resVal = req.op1/req.op2;
            sprintf(result, "%d", resVal);
        }else{
            sprintf(result, "Richiesta con soddisfabile... Ritenta!");
        }
        if(sendto(sock, result, sizeof(char[LINE_LENGTH]), 0, (struct sockaddr*)&clientaddr, len) < 0){
            perror("Errore in fase di sento");
            continue;
        }

    }


}