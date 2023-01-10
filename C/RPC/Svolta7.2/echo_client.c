#include <stdio.h>
#include <stdlib.h>
#include "echo.h"
#define DIM 100

//Da chiamare: operazioni_cliente <rpc host> <optype> <intero1> <intero2>

int main(int argc, char* argv[]){

    char *stringToSend;
    CLIENT* cl;
    char* host;
    char** stringReceived;

    if(argc != 2){
        printf("Inserisci la chiamata corretta: echo <rpc host>\n");
        exit(1);
    }

    host = argv[1];

    //Creo gestore trasporto
    cl = clnt_create(host, ECHOPROG, ECHOVERS, "udp");
    if(cl == NULL){
        clnt_pcreateerror(host); 
        exit(2);
    }

    stringToSend = (char*)malloc(DIM);
    printf("Inserisci la riga da inviare:\n");
    while(gets(stringToSend)){
        printf("Ho letto la stringa: %s\n", stringToSend);
        stringReceived = echo_1(&stringToSend, cl);
        printf("Dovrei aver ricevuto qualcosa...\n");
        //Gestione errori RPC
        if(stringReceived == NULL){
            fprintf(stderr, "%s: %s fallisce la rpc\n", argv[0], host);
            clnt_perror(cl, host);
            exit(3);
        }

        if(*stringReceived == NULL){
            fprintf(stderr, "%s: %s errore con la rpc\n", argv[0], host);
            exit(3);
        }

        printf("Il risultato è: %s\n", *stringReceived);
        printf("Inserisci la riga da inviare:\n");
    }
    //Libero il gestore di trasporto
    clnt_destroy(cl);
    free(stringToSend);


    exit(0);
}