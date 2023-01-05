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
#define BUFFER_SIZE 4096

// Client invocato con : ./Client <nome host> <porta>

int main(int argc, char **argv)
{

    int sock, fd_dest, nread, port;
    char buffer[BUFFER_SIZE];
    char file_name[FILENAME_MAX + 1];
    char new_file_name[FILENAME_MAX + 1];
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

    printf("Inserisci il nome del file di testo da trasferire:\n");
    while (gets(file_name))
    {
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

        printf("Inserisci il nome del nuovo file:\n");
        gets(new_file_name);
        if ((fd_dest = open(new_file_name, O_WRONLY | O_CREAT, 0644)) < 0)
        {
            perror("open");
            printf("Inserisci il nome del file di testo da trasferire:\n");
            continue;
        }

        if (write(sock, file_name, strlen(file_name) + 1) != strlen(file_name) + 1)
        {
            perror("write");
            printf("Inserisci il nome del file di testo da trasferire:\n");
            continue;
        }
        printf("Ho scritto nome file: %s, ora resto in attesa del file...\n", file_name);

        while ((nread = read(sock, buffer, sizeof(char[BUFFER_SIZE]))) > 0)
        {
            write(1, buffer, nread);
            write(fd_dest, buffer, nread);
        }

        close(fd_dest);
        close(sock);
        printf("File copiato con successo...\n");
        printf("Inserisci il nome del file di testo da trasferire:\n");
    }

}
