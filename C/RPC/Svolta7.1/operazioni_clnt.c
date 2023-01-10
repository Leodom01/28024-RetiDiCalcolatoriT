/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include <stdio.h>
#include <string.h>
#include "operazioni.h"

/* Default timeout can be changed using clnt_control() */
static struct timeval TIMEOUT = { 25, 0 };

int *
somma_1(argp, clnt)
	Operandi *argp;
	CLIENT *clnt;
{
	static int clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call(clnt, SOMMA, xdr_Operandi, argp, xdr_int, &clnt_res, TIMEOUT) != RPC_SUCCESS)
		return (NULL);
	return (&clnt_res);
}

int *
moltiplicazione_1(argp, clnt)
	Operandi *argp;
	CLIENT *clnt;
{
	static int clnt_res;

	memset((char *)&clnt_res, 0, sizeof(clnt_res));
	if (clnt_call(clnt, MOLTIPLICAZIONE, xdr_Operandi, argp, xdr_int, &clnt_res, TIMEOUT) != RPC_SUCCESS)
		return (NULL);
	return (&clnt_res);
}