#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xfactor.h"

// Da chiamare: c <rpc host>

int main(int argc, char *argv[])
{

    CLIENT *cl;
    char *host;
    char req[256];
    char singleChar;
    char cleaner;

    if (argc != 2)
    {
        printf("Inserisci la chiamata corretta: sala <rpc host>\n");
        exit(1);
    }

    host = argv[1];

    // Creo gestore trasporto
    cl = clnt_create(host, OPERAZIONIPROG, OPERAZIONIVERS, "udp");
    if (cl == NULL)
    {
        clnt_pcreateerror(host);
        exit(2);
    }

    printf("Inserisci G per vedere classifica giudici, T per classifica totale, V per votare:\n");
    while (gets(req))
    {
        if (strcmp(req, "G") == 0)
        {
            // Claffisica giudici
            printf("Preparati a vedere la classifica dei giudic:\n");

            Output *classifica;
            classifica = classifica_giudici_1(0, cl);

            if (classifica == NULL)
            {
                clnt_perror(cl, host);
                exit(3);
            }

            printf("Stampo la classifica giudici...\n");
            for (int i = 0; i < MAX_GIUDICI; i++)
            {
                printf("Posto %d: %s\n", i + 1, classifica->listaGiudici[i].lettera);
            }
        }
        else if (strcmp(req, "T") == 0)
        {
            // Classifica totale
            printf("Preparati alla classifica totale:\n");

            Tabella *table;
            table = gettabella_1(0, cl);

            if (table == NULL)
            {
                clnt_perror(cl, host);
                exit(3);
            }

            printf("Stampo la tabella...\n");
            for (int i = 0; i < TABELLA_SIZE; i++)
            {
                printf("Candidato: %s Giudice: %s Categoria: %c Nome file: %s Fase: %c Voto: %d\n",
                       table->riga[i].candidato, table->riga[i].giudice.lettera, table->riga[i].categoria, table->riga[i].filename, table->riga[i].fase, table->riga[i].voto);
            }
        }
        else if (strcmp(req, "V") == 0)
        {
            // Voto
            Input richiesta;
            printf("Inserisci il nome del partecipante:\n");
            gets(richiesta.candidato);
            printf("Inserisci A per +1 o S per -1:\n");
            gets(req);
            richiesta.operazione = req[0];

            printf("Mando richiesta");
            esprimi_voto_1(&richiesta, cl);

            printf("Voto inviato!\n");
        }
        printf("Inserisci G per vedere classifica giudici, T per classifica totale, V per votare:\n");
    }

    // Libero il gestore di trasporto
    clnt_destroy(cl);

    exit(0);
}