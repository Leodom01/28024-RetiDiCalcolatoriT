//File di implementazione server side delle procedure

#include "echo.h"
#include <stdio.h>
#include <rpc/rpc.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

char** echo_1_svc(char** string, struct svc_req* req){

    static char* stringToRet;
    //static char *echo_msg;

    free(stringToRet);
    //free(echo_msg);

    stringToRet = (char*)malloc(strlen(*string)+1); 
    //echo_msg = (char *)malloc(strlen(*string) + 1);

    strcpy(stringToRet, *string);
    //strcpy(echo_msg, *string);

    return (&stringToRet);


}
