struct Fila{ char posto[7];};
struct Sala{ Fila colonna[7];};
struct Prenotazione{ char tipo; int fila; int colonna;};

program OPERAZIONIPROG{
    version OPERAZIONIVERS{
        int PRENOTA_POSTAZIONE(Prenotazione) = 1;
        Sala VISUALIZZA_STATO() = 2;
    } = 1;
} = 0x20000013;