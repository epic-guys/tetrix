#ifndef _NETWORKING_H_
#define _NETWORKING_H_

#include <netinet/in.h>

typedef struct SrvConf
{
    int socket;
    int conn_socket;
    struct sockaddr_in client_addr;
} srvconf_t;

typedef struct GamePkg
{
    ushort type;
    ushort data_size;
    void* data;
} gamepkg_t;

srvconf_t host_game();
int connect_to_game(char* ip);
void close_server(srvconf_t server);
void close_client(int socket);
int is_an_ip(char* c);

char* recv_nickname(int socket);
int send_nickname(int socket, char* nickname);

#endif