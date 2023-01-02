/**
*	Dominici Leonardo Matricola: 0000971128
*
*	Library with multiple utilities to work in C with sockets (both UDP and TCP).
*   The offered "functions" are instead macro function so that the precompiler will resolve them 
*   adding no overhead whatsoever at runtime and not filling any function stack.
*
*   The library had been developed to ease the the UNIBO exams: 28024-Reti Di Calcolatori-T
*   (aka Computer networks for my international fellas [aka no one, but taht's another story])
*
*	@author:	leodom01
*	@GitHub:	https://github.com/Leodom01
*   @LinkedIn:  https://www.linkedin.com/in/leonardo-dominici/
*
*	Visit my GitHub page to download the last version of the lib
*	For problems or infos please contact me at: leonardodominici01@gmail.com or leonardo.dominici2@studio.unibo.it
*/

/**
 * Check the that the number of arguments is correct givent the argc taken from the main function.
 * If wrong prints the err_string.
 * 
 * @param arg_number    int: the main function argc
 * @param err_string    char*: the string to print in case of error before quitting
 * 
 * @return EXIT_FAILURE in case the args number does not match
*/
#define CHECK_ARGS_NUMBER(arg_number, err_string)                   \
    do{                                                             \
        if(argc != arg_number){                                     \
            printf("%s\n", err_string):                             \
            return EXIT_FAILURE                                     \
        }                                                           \
    }while(0)
                                                                 

/**
 * Check that the given value is greater that 0. If not it quits.
 * Useful to check socket opening, file opening and so on.
 * 
 * @param val          int: the value returned to check
 * @param err_string char*: the string to print if the value is less than 0 before quitting
 * 
 * @return EXIT_FAILURE in case the value is less than 0
*/
#define CHECK_VAL_NOT_LESS_0(val, err_string)                   \
    do{                                                         \
        if(val < 0){                                            \
            printf("%s\n", err_string);                         \
            return EXIT_FAILURE                                 \
        }                                                       \
    }while(0)

/**
 * Multiple checks on a string that should be representing a socket port number.
 * Check that it is correctly parsable to int and check that belongs to the allowed 
 * port number range (1025 to 65534). If checks are successful, fills the port_int pointer with the port int val
 * if not returns EXIT_FAILURE
 * 
 * @param port_arg      int: the string that represent the port to parse to int
 * @param err_string    char*: string to print before returning in case of error
 * @param port_int      int*: the var pointer that will be filled with the port int value
 *
 * @return EXIT_FAILURE in case some checks fail or if it is impossible to parse the port 
*/
#define CHECK_PORT(port_arg, err_string, port_int)                          \
    do{                                                                     \
        int port;                                                           \       
        int nread = 0;                                                      \
        while (port_arg[nread] != '\0') {                                   \
                if ((port_arg[nread] < '0') || (port_arg[nread] > '9')) {   \
                    printf("%s\n", err_string);                             \
                    printf("Error: %s port\n", port_arg);                   \
                    return EXIT_FAILURE                                     \
                }                                                           \
                nread++;                                                    \
        }                                                                   \
        port = atoi(port_arg);                                              \
        if (port < 1024 || port > 65535) {                                  \
                printf("%s\n", err_string);                                 \
                printf("Error: %s port\n", port_arg);                       \
                return EXIT_FAILURE                                         \
        }                                                                   \ 
        *port_int = port;                                                   \
    }while(0)                                                       
                                                            

/**
 * Opens a listening UDP socket at the given port creating a sockaddr_in as a support structure.
 * with the fist parameter (as a pointer). Fills in fd_toFill the socket file descriptor or print err_string
 * and then quit. The port is open with UDP protocol.
 * Makes server address init, socket creation and options setting, binding
 * 
 * @param port          int: the port to listen on 
 * @param err_string    char*: the string to print before exiting in case of error
 * @param fd_toFill     int*: the pointer to var that will be filled with the fd 
 * 
 * @return EXIT_FAILURE in case of error 
*/
#define SERVER_SOCK_UDP_INIT(port, err_string, fd_toFill)                                                       \                      
    do{                                                                                                         \
        //Initialize thisAddr struct with socket number                                                         \       
        struct sockaddr_in thisAddr;                                                                            \            
        const int          on = 1;                                                                              \
        memset((char*)&thisAddr, 0, sizeof(thisAddr));                                                          \            
        thisAddr.sin_family      = AF_INET;                                                                     \        
        thisAddr.sin_addr.s_addr = INADDR_ANY;                                                                  \        
        thisAddr.sin_port        = htons(port);                                                                 \
        //Spawn filedescriptor from socket primitive                                                            \            
        *fd_toFill = socket(AF_INET, SOCK_DGRAM, 0);                                                            \
        char errorString[2048];                                                                                 \        
        sprintf(errorString, "Error while calling socket to open fd while UDP connection at port %d\n", port);  \        
        CHECK_VAL_NOT_LESS_0(*fd_toFill, errorString);                                                          \
        if(setsockopt(*fd_toFill, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0){                              \
            sprintf(errorString, "Error while setting socket options on port %d", port);                        \
            printf("%s", errorString);                                                                          \
        }                                                                                                       \
        if (bind(*fd_toFill, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {                             \
            sprintf(errorString, "Error while binding socket on port %d", port);                                \
            printf("%s", errorString);                                                                          \
        }                                                                                                       \
    }while(0)


/**
 * Opens a listening TCP socket at the given port creating a sockaddr_in as a support structure.
 * with the fist parameter (as a pointer). Fills in fd_toFill the socket file descriptor or print err_string
 * and then quit. The port is open with TCP protocol.
 * Makes server address init, socket creation and options setting, binding
 * 
 * @param port          int: the port to listen on 
 * @param err_string    char*: the string to print before exiting in case of error
 * @param fd_toFill     int*: the pointer to var that will be filled with the fd 
 * 
 * @return EXIT_FAILURE in case of error 
*/
#define SERVER_SOCK_TCP_INIT(port, err_string, fd_toFill)                                                       \                      
    do{                                                                                                         \
        //Initialize thisAddr struct with socket number                                                         \       
        struct sockaddr_in thisAddr;                                                                            \            
        const int          on = 1;                                                                              \            
        memset((char*)&thisAddr, 0, sizeof(thisAddr));                                                          \            
        thisAddr.sin_family      = AF_INET;                                                                     \        
        thisAddr.sin_addr.s_addr = INADDR_ANY;                                                                  \        
        thisAddr.sin_port        = htons(port);                                                                 \
        //Spawn filedescriptor from socket primitive                                                            \            
        *fd_toFill = socket(AF_INET, SOCK_STREAM, 0);                                                           \
        char errorString[2048];                                                                                 \        
        sprintf(errorString, "Error while calling socket to open fd while TCP connection at port %d\n", port);  \        
        CHECK_VAL_NOT_LESS_0(*fd_toFill, errorString);                                                          \
        if(setsockopt(*fd_toFill, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0){                              \
            sprintf(errorString, "Error while setting socket options on port %d", port);                        \
            printf("%s", errorString);                                                                          \
        }                                                                                                       \
        if (bind(*fd_toFill, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {                             \
            sprintf(errorString, "Error while binding socket on port %d", port);                                \
            printf("%s", errorString);                                                                          \
        }                                                                                                       \
    }while(0)