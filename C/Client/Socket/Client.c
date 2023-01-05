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

// Chiama con ./Client <host> <serverPort>

int main(int argc, char **argv)
{
    int sock, fd_sorg, fd_dest, nread, port;
    char buffer[LINE_LENGTH];
    char nome_sorg[FILENAME_MAX + 1], nome_dest[FILENAME_MAX + 1];
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

    printf("Inserisci il nome del file da ordinare, EOF per uscire:\n");
    while (gets(nome_sorg))
    {
        printf("File da ordinare: %s\n", nome_sorg);
        if ((fd_sorg = open(nome_sorg, O_RDONLY)) < 0)
        {
            printf("Aperura file non compiuta, riprova...\n");
            printf("Inserisci il nome del file da ordinare, EOF per uscire:\n");
            continue;
        }
        printf("Inserisci il nome del file destinazione:\n");
        gets(nome_dest);
        printf("File destinazione: %s\n", nome_dest);
        if ((fd_dest = open(nome_dest, O_WRONLY | O_CREAT, 0644)) < 0)
        {
            printf("Aperura file non compiuta, riprova...\n");
            printf("Inserisci il nome del file da ordinare, EOF per uscire:\n");
            continue;
        }

        printf("Socket connessa, inizio a scrivree dati...\n");
        while ((nread = read(fd_sorg, buffer, LINE_LENGTH)) > 0)
        {
            write(sock, buffer, nread);
            write(1, buffer, nread); // Stampo a console
        }
        shutdown(sock, 1); // Chiudo lato di scrittura siccome non lo uso più
        printf("\nOra inizio lettura del file sortato...\n");
        while ((nread = read(sock, buffer, LINE_LENGTH)) > 0)
        {
            write(1, buffer, nread);
            write(fd_dest, buffer, nread);
        }
        shutdown(sock, 0); // Chiudo lato lettura
        close(fd_sorg);
        close(fd_dest);
        close(sock);
        printf("Ordinamento completato, ora il prossimo o esci...\n");
        printf("Inserisci il nome del file da ordinare, EOF per uscire:\n");
        continue;
    }

    exit(0);
}
