#ifndef _NETWORKING_H_
#define _NETWORKING_H_

#include <netinet/in.h>

typedef struct SrvConf srvconf_t;


srvconf_t host_game();
int is_an_ip(char* c);

#endif