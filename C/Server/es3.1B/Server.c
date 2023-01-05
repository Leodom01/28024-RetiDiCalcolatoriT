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

#define LINE_LENGTH 1023

// Da chiamare come: ./Server <porta da esporre>

//In caso in cui il main thread generi dei figli, poi ricordati di fare signal(gestore, SIGCHLD)
void gestore(int signo){
    int stato;
    printf("In esecuzione gestore di SIGCHLD\n");
    wait(&stato);
}

int main(int argc, char **argv) {
    

    int sock, open_conn_sock, line_to_del, nread, lineCounter; 
    int port, len;
    const int on = 1;
    char buffer[LINE_LENGTH];
    char outbuffer[LINE_LENGTH+1];
    struct sockaddr_in servaddr, clientaddr;
    struct hostent* host;


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
    printf("Socket creata\n");

    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
        perror("etsockopt");
        exit(2);
    }
    printf("Socket opt settate\n");

    if (bind(sock, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
    {
        perror("bind");
        exit(2);
    }
    printf("Bind successful\n");

    if (listen(sock, 5) < 0)
    {
        perror("listen");
        exit(2);
    }
    printf("Coda d'ascolto creata\n");

    signal(SIGCHLD, gestore);

    while(1){
        printf("Inizio a fare accept");
        len = sizeof(clientaddr);
        if((open_conn_sock = accept(sock, (struct sockaddr*)&clientaddr, &len)) < 0){
            printf("Open minore di 1");
            if (errno == EINTR)
            {
                printf("Dentro a errno == EINTR\n");
                perror("accept");
                continue;
            }
            else
            {
                printf("Tocca uscire\n");
                exit(2);
            }
        }

        printf("Accept fatta con successo, ora entro nel figlio...\n");
        if(fork() == 0){
            //Figlio
            if((nread = read(open_conn_sock, &line_to_del, sizeof(line_to_del))) < 0){
                perror("read");
                close(open_conn_sock);
                exit(3);
            }
            
            lineCounter = 0;
            int outbufferLen = 0;
            printf("FIGLIO: Mi dicono di voler cancellare la riga %d\n", line_to_del);
            while((nread = read(open_conn_sock, buffer, LINE_LENGTH)) > 0){
                printf("Appena letto: %s\n", buffer);
                outbufferLen = 0;
                for(int i = 0; i<nread; i++){
                    if(buffer[i] == '\n'){
                        lineCounter++;
                    }
                    if(lineCounter != line_to_del){
                        outbuffer[outbufferLen] = buffer[i];
                        outbufferLen++;
                    }
                }
                outbuffer[outbufferLen] = '\n';
                write(open_conn_sock, outbuffer, sizeof(char[outbufferLen]));
                printf("Appena scritto %s\n", outbuffer);
            }
            shutdown(open_conn_sock, SHUT_RDWR);
            close(open_conn_sock);
            printf("FIGLIO: Fine\n");
            exit(0);
        }
        close(open_conn_sock);
    }

}

