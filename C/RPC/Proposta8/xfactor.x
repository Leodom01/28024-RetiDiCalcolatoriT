const TABELLA_SIZE = 5;
const MAX_GIUDICI = 5;

struct Giudice{char lettera[30];};
struct Item{char candidato[30]; Giudice giudice; char categoria; char filename[64]; char fase; int voto;};
struct Tabella{Item riga[TABELLA_SIZE];};
struct Output{Giudice listaGiudici[MAX_GIUDICI];};
struct Input{char candidato[30]; char operazione;};

program OPERAZIONIPROG{
    version OPERAZIONIVERS{
        Output CLASSIFICA_GIUDICI() = 1;
        void ESPRIMI_VOTO(Input) = 2;
        Tabella GETTABELLA() = 3;
    } = 1;
} = 0x20000013;