/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _SALA_H_RPCGEN
#define _SALA_H_RPCGEN

#define RPCGEN_VERSION	199506

#include <rpc/rpc.h>


struct Fila {
	char posto[7];
};
typedef struct Fila Fila;
#ifdef __cplusplus
extern "C" bool_t xdr_Fila(XDR *, Fila*);
#elif __STDC__
extern  bool_t xdr_Fila(XDR *, Fila*);
#else /* Old Style C */
bool_t xdr_Fila();
#endif /* Old Style C */


struct Sala {
	Fila colonna[7];
};
typedef struct Sala Sala;
#ifdef __cplusplus
extern "C" bool_t xdr_Sala(XDR *, Sala*);
#elif __STDC__
extern  bool_t xdr_Sala(XDR *, Sala*);
#else /* Old Style C */
bool_t xdr_Sala();
#endif /* Old Style C */


struct Prenotazione {
	char tipo;
	int fila;
	int colonna;
};
typedef struct Prenotazione Prenotazione;
#ifdef __cplusplus
extern "C" bool_t xdr_Prenotazione(XDR *, Prenotazione*);
#elif __STDC__
extern  bool_t xdr_Prenotazione(XDR *, Prenotazione*);
#else /* Old Style C */
bool_t xdr_Prenotazione();
#endif /* Old Style C */


#define OPERAZIONIPROG ((rpc_uint)0x20000013)
#define OPERAZIONIVERS ((rpc_uint)1)

#ifdef __cplusplus
#define PRENOTA_POSTAZIONE ((rpc_uint)1)
extern "C" int * prenota_postazione_1(Prenotazione *, CLIENT *);
extern "C" int * prenota_postazione_1_svc(Prenotazione *, struct svc_req *);
#define VISUALIZZA_STATO ((rpc_uint)2)
extern "C" Sala * visualizza_stato_1(void *, CLIENT *);
extern "C" Sala * visualizza_stato_1_svc(void *, struct svc_req *);

#elif __STDC__
#define PRENOTA_POSTAZIONE ((rpc_uint)1)
extern  int * prenota_postazione_1(Prenotazione *, CLIENT *);
extern  int * prenota_postazione_1_svc(Prenotazione *, struct svc_req *);
#define VISUALIZZA_STATO ((rpc_uint)2)
extern  Sala * visualizza_stato_1(void *, CLIENT *);
extern  Sala * visualizza_stato_1_svc(void *, struct svc_req *);

#else /* Old Style C */
#define PRENOTA_POSTAZIONE ((rpc_uint)1)
extern  int * prenota_postazione_1();
extern  int * prenota_postazione_1_svc();
#define VISUALIZZA_STATO ((rpc_uint)2)
extern  Sala * visualizza_stato_1();
extern  Sala * visualizza_stato_1_svc();
#endif /* Old Style C */

#endif /* !_SALA_H_RPCGEN */