/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "operazioni.h"
#include <unistd.h>

bool_t
xdr_Operandi(xdrs, objp)
	XDR *xdrs;
	Operandi *objp;
{

	if (!xdr_int(xdrs, &objp->op1))
		return (FALSE);
	if (!xdr_int(xdrs, &objp->op2))
		return (FALSE);
	return (TRUE);
}
