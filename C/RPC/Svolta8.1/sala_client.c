#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sala.h"

//Da chiamare: sala <rpc host>

int main(int argc, char* argv[]){

    Prenotazione prenot;
    Sala* sal;
    int* prenot_result;
    CLIENT* cl;
    char* host;
    char req[256];
    char cleaner;

    if(argc != 2){
        printf("Inserisci la chiamata corretta: sala <rpc host>\n");
        exit(1);
    }

    host = argv[1];

    //Creo gestore trasporto
    cl = clnt_create(host, OPERAZIONIPROG, OPERAZIONIVERS, "udp");
    if(cl == NULL){
        clnt_pcreateerror(host); 
        exit(2);
    }

    printf("Inserisci P per prenotare, V per visualizzare, EOF per temrinare:\n");
    while(gets(req)){
        if(strcmp(req, "P") == 0){
            //Prenotazione
            printf("Inserisci B, P, D\n");
            //Dovrei fare controllo
            prenot.tipo = getchar();
            getchar(); //Consumo finelinea
            printf("Inserisci la fila:\n");
            //Dovrei fare controllo
            scanf("%d", &(prenot.fila));
            printf("Inserisci la riga:\n");
            //Dovrei fare controllo
            scanf("%d", &(prenot.colonna));
            do{
                cleaner = getchar();
                printf("Ho letto\n");
            }while(cleaner != '\n');

            printf("Mi preparo a prenotare posto %c in fila %d e riga %d\n", prenot.tipo, prenot.fila, prenot.colonna);

            prenot_result = prenota_postazione_1(&prenot, cl);

            //Gestione errori RPC
            if(prenot_result == NULL){
                clnt_perror(cl, host);
                exit(3);
             }

            printf("Ho ricevuto come risposta %d...\n", *prenot_result);
            printf("Inserisci P per prenotare, V per visualizzare, EOF per temrinare:\n");
        }else if(strcmp(req, "V") == 0){
            //Visualizzazione
            printf("Faccio richiesta di visualizzazione della sala...\n");
            sal = visualizza_stato_1(0, cl);

            //Gestione errori RPC
            if(sal == NULL){
                clnt_perror(cl, host);
                exit(3);
             }

            for(int i = 0; i<7; i++){
                printf("\nFila %d: ", i+1);
                for(int j = 0; j<7; j++){
                    printf(" %c ", sal->colonna[j].posto[i]);
                    //Verifica che gli indici siano nell'ordine giusto
                }
            }

            printf("\nTerminato di stampare la struttura dati...\n");
            printf("Inserisci P per prenotare, V per visualizzare, EOF per temrinare:\n");
        }else{
            printf("Inserisci P per prenotare, V per visualizzare, EOF per temrinare:");
        }
    }

    

    //Libero il gestore di trasporto
    clnt_destroy(cl);


    exit(0);
}