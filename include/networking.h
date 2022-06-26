#ifndef _NETWORKING_H_
#define _NETWORKING_H_

#include <netinet/in.h>

typedef struct SrvConf
{
    int socket;
    int conn_socket;
    struct sockaddr_in client_addr;
} srvconf_t;

/**
 * Un pacchetto da inviare in rete.
 * @property type Che cosa contiene il pacchetto.
 * @property size La dimensione del contenuto.
 * @property data Il contenuto.
 */
typedef struct GamePkg
{
    ushort type;
    ushort size;
    void* data;
} gamepkg_t;

int host_game();
int connect_to_game(char* ip);
void close_server(srvconf_t server);
void close_client(int socket);
int is_an_ip(char* c);
gamepkg_t unpack_pkg(void* buff);

char* recv_nickname(int socket);
int send_nickname(int socket, char* nickname);
int* recv_field(int socket);
int send_field(int socket, int* field);
int send_tet_type(int socket, int type);
int recv_tet_type(int socket);
int send_start_game(int socket, int starting);
int recv_start_game(int socket);
int send_added_tet(int socket, int added);
int recv_added_tet(int socket);
#endif