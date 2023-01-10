//File di implementazione server side delle procedure

#include <stdio.h>
#include <rpc/rpc.h>
#include <unistd.h>
#include "sala.h"

static Sala sala;
static int inited = 0;

void initSala(){
    if(inited == 0){
        for(int i = 0; i<7; i++){
            for(int j = 0; j<7; j++){
                sala.colonna[i].posto[j] = 'L';
            }
        }
        inited = 1;
    }
    return;
}

int* prenota_postazione_1_svc(Prenotazione* prenot, struct svc_req* req){
    initSala();

    static int toRet;

    if(sala.colonna[prenot->colonna].posto[prenot->fila] == 'L'){
        sala.colonna[prenot->colonna].posto[prenot->fila] = prenot->tipo;
        toRet = 0;
    }else{
        toRet = -1; 
    }
    
    return (&toRet);
}

Sala* visualizza_stato_1_svc(void* none, struct svc_req* req){
    initSala();

    static Sala toRet; 
     for(int i = 0; i<7; i++){
        for(int j = 0; j<7; j++){
            toRet.colonna[i].posto[j] = sala.colonna[i].posto[j];
        }
    }

    return (&toRet);
}