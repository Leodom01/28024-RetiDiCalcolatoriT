// File di implementazione server side delle procedure

#include <stdio.h>
#include <rpc/rpc.h>
#include <unistd.h>
#include <string.h>
#include "xfactor.h"

static Tabella tabella;
static int inited = 0;

void init()
{
    if (inited == 0)
    {
        for (int line = 0; line < TABELLA_SIZE; line++)
        {
            strcpy(tabella.riga[line].candidato, "L");
            strcpy(tabella.riga[line].giudice.lettera, "L");
            tabella.riga[line].categoria = 'L';
            strcpy(tabella.riga[line].filename, "L");
            tabella.riga[line].fase = 'L';
            tabella.riga[line].voto = -1;
        }

        // Riempio riga 0
        strcpy(tabella.riga[0].candidato, "Candidato0");
        strcpy(tabella.riga[0].giudice.lettera, "Giudice03");
        tabella.riga[0].categoria = 'U';
        strcpy(tabella.riga[0].filename, "cand0.txt");
        tabella.riga[0].fase = 'S';
        tabella.riga[0].voto = 0;

        // Riempio riga 2
        strcpy(tabella.riga[2].candidato, "Candidato2");
        strcpy(tabella.riga[2].giudice.lettera, "Giudice2");
        tabella.riga[2].categoria = 'D';
        strcpy(tabella.riga[2].filename, "cand2.txt");
        tabella.riga[2].fase = 'B';
        tabella.riga[2].voto = 0;

        // Riempio riga 3
        strcpy(tabella.riga[3].candidato, "Candidato3");
        strcpy(tabella.riga[3].giudice.lettera, "Giudice03");
        tabella.riga[3].categoria = 'D';
        strcpy(tabella.riga[3].filename, "cand3.txt");
        tabella.riga[3].fase = 'S';
        tabella.riga[3].voto = 0;

        // Riempio riga 4
        strcpy(tabella.riga[4].candidato, "Candidato4");
        strcpy(tabella.riga[4].giudice.lettera, "Giudice4");
        tabella.riga[4].categoria = 'O';
        strcpy(tabella.riga[4].filename, "cand4.txt");
        tabella.riga[4].fase = 'S';
        tabella.riga[4].voto = 0;

        inited = 1;
    }
    return;
}

Output *classifica_giudici_1_svc(void *nothing, struct svc_req *req)
{
    init();

    int voti[MAX_GIUDICI];
    Output classifica;
    static Output classificaDefinitiva;
    int indexMax;

    int maxVoto = 0;
    for(int i = 0; i<MAX_GIUDICI; i++){
        voti[i] = 0;
        strcpy(classificaDefinitiva.listaGiudici[i].lettera, "L");
        strcpy(classifica.listaGiudici[i].lettera, "L");
    }

    for(int index = 0; index<TABELLA_SIZE; index++){
        for(int i = 0; i<MAX_GIUDICI; i++){
            if(strcmp(tabella.riga[index].giudice.lettera, classifica.listaGiudici[i].lettera) == 0){
                voti[i] += tabella.riga[index].voto;
                break;
            }else if(strcmp("L", classifica.listaGiudici[i].lettera) == 0){
                strcpy(classifica.listaGiudici[i].lettera, tabella.riga[index].giudice.lettera);
                voti[i] += tabella.riga[index].voto;
                break;
            }
        }
    }

    for(int i = 0; i<MAX_GIUDICI; i++){
        maxVoto = -1;
        indexMax = -1;
        //Fill max voto
        for(int j = 0; j<MAX_GIUDICI; j++){
            if(voti[j] > maxVoto){
                maxVoto = voti[j];
                indexMax = j;
            }
        }

        //Cerca primo spaio libero in classifica e fillalo
        for(int j = 0; j<MAX_GIUDICI; j++){
            if(strcmp(classificaDefinitiva.listaGiudici[j].lettera, "L") == 0){
                strcpy(classificaDefinitiva.listaGiudici[j].lettera, classifica.listaGiudici[indexMax].lettera);
                break;
            }
        }
    }

    return (&classificaDefinitiva);
}

void* esprimi_voto_1_svc(Input* inpt, struct svc_req *req)
{
    init();
    for(int i = 0; i<TABELLA_SIZE; i++){
        if(strcmp(tabella.riga[i].candidato, inpt->candidato) == 0){
            if(inpt->operazione == 'A'){
                //Aggiungi voto
                tabella.riga[i].voto++;
            }else if(inpt->operazione == 'S'){
                //Sottrai voto
                tabella.riga[i].voto--;
            }
            break;
        }
    }

    return 0;
}

Tabella* gettabella_1_svc(void* nothing, struct svc_req* req){
    init();

    return (&tabella);
}