#include <stdio.h>
#include <stdlib.h>
#include "operazioni.h"

//Da chiamare: operazioni_cliente <rpc host> <optype> <intero1> <intero2>

int main(int argc, char* argv[]){

    Operandi op;
    CLIENT* cl;
    char* host;
    int* risultato;

    if(argc != 5){
        printf("Inserisci la chiamata corretta: operazioni_cliente <rpc host> <optype> <intero1> <intero2>\n");
        exit(1);
    }else if(argv[2][0] != 's' && argv[2][0] != 'm'){
        printf("+ o * per indicare la operazione...\n");
        printf("Inserisci la chiamata corretta: operazioni_cliente <rpc host> <optype> <intero1> <intero2>\n");
        exit(1);  
    };

    op.op1 = atoi(argv[3]);
    op.op2 = atoi(argv[4]);
    host = argv[1];

    //Creo gestore trasporto
    cl = clnt_create(host, OPERAZIONIPROG, OPERAZIONIVERS, "udp");
    if(cl == NULL){
        clnt_pcreateerror(host); 
        exit(2);
    }

    if(argv[2][0] == 's'){
        risultato = somma_1(&op, cl);
    }else if(argv[2][0] == 'm'){
        risultato = moltiplicazione_1(&op, cl);
    }

    //Gestione errori RPC
    if(risultato == NULL){
        clnt_perror(cl, host);
        exit(3);
    }

    printf("Il risultato è: %d\n", *risultato);

    //Libero il gestore di trasporto
    clnt_destroy(cl);


    exit(0);
}