/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _OPERAZIONI_H_RPCGEN
#define _OPERAZIONI_H_RPCGEN

#define RPCGEN_VERSION	199506

#include <rpc/rpc.h>


struct Operandi {
	int op1;
	int op2;
};
typedef struct Operandi Operandi;
#ifdef __cplusplus
extern "C" bool_t xdr_Operandi(XDR *, Operandi*);
#elif __STDC__
extern  bool_t xdr_Operandi(XDR *, Operandi*);
#else /* Old Style C */
bool_t xdr_Operandi();
#endif /* Old Style C */


#define OPERAZIONIPROG ((rpc_uint)0x20000013)
#define OPERAZIONIVERS ((rpc_uint)1)

#ifdef __cplusplus
#define SOMMA ((rpc_uint)1)
extern "C" int * somma_1(Operandi *, CLIENT *);
extern "C" int * somma_1_svc(Operandi *, struct svc_req *);
#define MOLTIPLICAZIONE ((rpc_uint)2)
extern "C" int * moltiplicazione_1(Operandi *, CLIENT *);
extern "C" int * moltiplicazione_1_svc(Operandi *, struct svc_req *);

#elif __STDC__
#define SOMMA ((rpc_uint)1)
extern  int * somma_1(Operandi *, CLIENT *);
extern  int * somma_1_svc(Operandi *, struct svc_req *);
#define MOLTIPLICAZIONE ((rpc_uint)2)
extern  int * moltiplicazione_1(Operandi *, CLIENT *);
extern  int * moltiplicazione_1_svc(Operandi *, struct svc_req *);

#else /* Old Style C */
#define SOMMA ((rpc_uint)1)
extern  int * somma_1();
extern  int * somma_1_svc();
#define MOLTIPLICAZIONE ((rpc_uint)2)
extern  int * moltiplicazione_1();
extern  int * moltiplicazione_1_svc();
#endif /* Old Style C */

#endif /* !_OPERAZIONI_H_RPCGEN */
