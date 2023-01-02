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

// Da chiamare come: ./Server <porta da esporre>

// Appena un figlio termina triggera SIGCHLD e io leggo con wait il risultato così il figlio muore e non resta zombie
void gestore(int signo)
{
    int stato;
    printf("Eseguo gestore di SIGCHLD\n");
    wait(&stato);
}

int main(int argc, char **argv)
{

    int sock, open_conn_sock;
    int port, len;
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

    while (1)
    {
        printf("Mi preparo a fare le accept...\n");
        len = sizeof(clientaddr);
        if ((open_conn_sock = accept(sock, (struct sockaddr *)&clientaddr, &len)) < 0)
        {
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
        printf("Passata una accept con successo, si va...\n");
        if (fork() == 0) // Figlio
        {
            printf("Sono dentro al figlio!\n");
            // Chiusura file descriptor non utilizzati e ridirezione di stdin e stdout
            close(sock);
            close(1);
            close(0);
            dup(open_conn_sock);
            dup(open_conn_sock);
            close(open_conn_sock);
            // Esecuzione ordinamento
            execl("/usr/bin/sort", "sort", (char *)0);
        }
        // come se ci fosse un: else
        // PADRE: chiusura socket di connessione (NON di ascolto)
        close(open_conn_sock);
    }
}
