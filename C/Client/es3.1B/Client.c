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

// Chiama con ./Client <hostname> <serverPort>

int main(int argc, char **argv)
{
    int sock, fd_file_in, fd_file_out, nread, port, line, filesize;
    char buffer[LINE_LENGTH];
    char filename_in[FILENAME_MAX + 1], filename_out[FILENAME_MAX + 1];
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

    memset((char *)&servaddr, 0, sizeof(struct sockaddr_in));
    servaddr.sin_family = AF_INET;
    host = gethostbyname(argv[1]);
    if (host == NULL)
    {
        perror("gethostbyname");
        exit(2);
    }
    printf("Host info, name: %s, ip: %s\n", host->h_name, host->h_addr_list[1]);
    servaddr.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr;
    printf("Chiamo su %d\n", port);
    servaddr.sin_port = htons(port);

    printf("Inserisci il nome del file da leggere:\n");
    while (gets(filename_in) != 0)
    {
        if ((fd_file_in = open(filename_in, O_RDONLY)) < 0)
        {
            printf("Aperura file non compiuta, riprova...\n");
            printf("Inserisci il nome del file da ordinare, EOF per uscire:\n");
            continue;
        }
        printf("Inserisci la riga del file da eliminare: \n");
        scanf("%d", &line);

        // Invio filesize, poi riga da eliminare e poi contenuto
        filesize = 0;
        while ((nread = read(fd_file_in, buffer, LINE_LENGTH)) > 0)
        {
            filesize += nread;
        }
        close(fd_file_in);
        if ((fd_file_in = open(filename_in, O_RDONLY)) < 0)
        {
            printf("Aperura file non compiuta, riprova...\n");
            printf("Inserisci il nome del file da ordinare, EOF per uscire:\n");
            continue;
        }

        // Apro e bindo socket
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
        {
            perror("socket");
            printf("Inserisci il nome del file da ordinare, EOF per uscire:\n");
            exit(2);
        }
        if (connect(sock, (struct sockaddr *)&servaddr, sizeof(struct sockaddr)) < 0)
        {
            perror("connect");
            printf("Inserisci il nome del file da ordinare, EOF per uscire:\n");
            exit(2);
        }

        // Invio filesize
        //if ((nread = write(sock, &filesize, sizeof(filesize))) < 0)
        //{
        //    perror("write");
        //    close(fd_file_in);
        //    printf("Inserisci il nome del file da ordinare, EOF per uscire:\n");
        //    continue;
        //}

        // Invio riga da eliminare
        if ((nread = write(sock, &line, sizeof(line))) < 0)
        {
            perror("write");
            close(fd_file_in);
            printf("Inserisci il nome del file da ordinare, EOF per uscire:\n");
            continue;
        }

        // Invio tutto il file ora
        while ((nread = read(fd_file_in, buffer, LINE_LENGTH)) > 0)
        {
            if (write(sock, buffer, nread) != nread)
            {
                perror("write");
                close(fd_file_in);
                printf("Inserisci il nome del file da ordinare, EOF per uscire:\n");
                continue;
            }
            printf("Inviato: %s\n", buffer);
        }
        shutdown(sock, SHUT_WR);
        fd_file_out = open("result.txt", O_WRONLY | O_CREAT, 0644);
        // Attendo il file risultato
        printf("Ecco il risultato...\n");
        while ((nread = read(sock, buffer, LINE_LENGTH)) > 0)
        {
            printf("Leggo %d caratteri: %s\n", nread, buffer);
            write(fd_file_out, buffer, nread);
        }
        printf("\nLettura del file finita.\n");
        shutdown(sock, SHUT_RD);
        close(fd_file_out);
        close(fd_file_in);
        close(sock);

        printf("Inserisci il nome del file da leggere:\n");
    }
}
