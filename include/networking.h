#ifndef _NETWORKING_H_
#define _NETWORKING_H_

#include <netinet/in.h>

typedef struct SrvConf
{
    int socket;
    int conn_socket;
    struct sockaddr_in client_addr;
} srvconf_t;


srvconf_t host_game();


#endif