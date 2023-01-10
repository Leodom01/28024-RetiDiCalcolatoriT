//File di implementazione server side delle procedure

#include <stdio.h>
#include <rpc/rpc.h>
#include <unistd.h>
#include "operazioni.h"

int* somma_1_svc(Operandi* op, struct svc_req* req){

    printf("Ricevuta richiesta somma con operandi: %d %d\n", op->op1, op->op2);
    static int result; 

    result = (op->op1) + (op->op2);
    printf("Ritorno risultato da somma: %d\n", result);
    fflush(stdout);
    return (&result);
}

int* moltiplicazione_1_svc(Operandi* op, struct svc_req* req){
    printf("Ricevuta richiesta moltiplicazione con operandi: %d %d\n", op->op1, op->op2);
    static int result; 

    result = (op->op1) * (op->op2);
    printf("Ritorno risultato da somma: %d\n", result);
    fflush(stdout);
    return (&result);
}