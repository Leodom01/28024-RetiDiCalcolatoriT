/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _ECHO_H_RPCGEN
#define _ECHO_H_RPCGEN

#define RPCGEN_VERSION	199506

#include <rpc/rpc.h>


#define ECHOPROG ((rpc_uint)0x20000013)
#define ECHOVERS ((rpc_uint)1)

#ifdef __cplusplus
#define ECHO ((rpc_uint)1)
extern "C" char ** echo_1(char **, CLIENT *);
extern "C" char ** echo_1_svc(char **, struct svc_req *);

#elif __STDC__
#define ECHO ((rpc_uint)1)
extern  char ** echo_1(char **, CLIENT *);
extern  char ** echo_1_svc(char **, struct svc_req *);

#else /* Old Style C */
#define ECHO ((rpc_uint)1)
extern  char ** echo_1();
extern  char ** echo_1_svc();
#endif /* Old Style C */

#endif /* !_ECHO_H_RPCGEN */