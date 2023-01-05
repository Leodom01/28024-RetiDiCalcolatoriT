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

// In caso in cui il main thread generi dei figli, poi ricordati di fare signal(gestore, SIGCHLD)
void gestore(int signo)
{
    int stato;
    printf("In esecuzione gestore di SIGCHLD\n");
    wait(&stato);
    printf("stato figlio: %d\n", stato >> 8);
}

int main(int argc, char **argv)
{   
    int port, nread;

    //Controllo correttezza argomento porta
    nread = 0;
    while (argv[2][nread] != '\0')
    {
        if ((argv[2][nread] < '0') || (argv[2][nread] > '9'))
        {
            printf("Secondo argomento non intero\n");
            printf("Error:%s serverAddress serverPort\n", argv[0]);
            exit(2);
        }
        nread++;
    }
    port = atoi(argv[2]);
    if (port < 1024 || port > 65535)
    {
        printf("Port scorretta...");
        exit(2);
    }
}
