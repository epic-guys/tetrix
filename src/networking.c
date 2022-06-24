/* Serve per il socket */
#include <sys/socket.h>
/* Tipi della rete, ad esempio AF_INET */
#include <sys/types.h>
/* Struct per l'indirizzo */
#include <netinet/in.h>
/* Funzione per convertire gli indirizzi*/
#include <arpa/inet.h>
/* Chiusura del file descriptor dello stream */
#include <unistd.h>
#include <errno.h>

#include "constants.h"
#include "networking.h"

typedef struct SrvConf
{
    int socket;
    int conn_socket;
    struct sockaddr_in client_addr;
} srvconf_t;

/**
 * @brief Apre la connessione in entrata per 
 * 
 * @return Il socket della connessione con il client.
 */
srvconf_t host_game()
{
    struct sockaddr_in addr;
    int err;
    srvconf_t conf;

    conf.socket = socket(AF_INET, SOCK_STREAM, 0);
    if (conf.socket == -1)
    {
        /* Errore */
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(conf.socket, (struct sockaddr*) &addr, sizeof(struct sockaddr_in));
    if (bind == -1)
    {
        /* Errore */
    }
    
    err = listen(conf.socket, 1);
    if (err == -1)
    {
        /* Errore */
    }

    conf.conn_socket = accept(conf.socket, (struct sockaddr*) &conf.conn_socket, sizeof(struct sockaddr_in));

    if (conf.conn_socket == -1)
    {
        /* Errore */
    }

    return conf;
}

void close_server(srvconf_t server)
{
    close(server.conn_socket);
    close(server.socket);
}

int connect_to_game(char* addr)
{
    
}

int* discover_games()
{
    
}