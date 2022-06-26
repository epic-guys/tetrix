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
/*Gestione dei caratteri*/
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "../include/constants.h"
#include "../include/networking.h"

#define pkg_data_size(buffer) (buffer - sizeof(short))

/**
 * 
 * @brief Apre la connessione in entrata per 
 * 
 * @return Il socket della connessione con il client.
 */
srvconf_t host_game()
{
    struct sockaddr_in addr;
    socklen_t addr_len;
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

    err = bind(conf.socket, (struct sockaddr*) &addr, sizeof(struct sockaddr_in));
    if (err == -1)
    {
        /* Errore */
    }
    
    err = listen(conf.socket, 1);
    if (err == -1)
    {
        /* Errore */
    }

    conf.conn_socket = accept(conf.socket, (struct sockaddr*) &conf.client_addr, &addr_len);

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

void close_client(int socket)
{
    close(socket);
}

/**
 * @brief Si connette alla partita all'IP specificato.
 * 
 * @param ip L'indirizzo IP a cui connettersi.
 * @return Il numero del socket.
 */
int connect_to_game(char* ip)
{
    struct sockaddr_in addr;
    socklen_t addr_len;
    int sock, err;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(PORT);

    err = connect(sock, (struct sockaddr*) &addr, sizeof(addr));
    if (err == -1)
    {
        /* ERRORE */
    }

    return sock;
}

gamepkg_t unpack_pkg(void* buff, size_t size)
{
    gamepkg_t pkg;
    memmove(&pkg, buff, size);    
    return pkg;
}

void* pack_pkg(gamepkg_t pkg, size_t size)
{
    void* mem = malloc(size);
    memmove(mem, &pkg.type, sizeof(ushort));
    memmove(mem + sizeof(ushort), pkg.data, size - sizeof(ushort));
    return mem;
}

int is_an_ip(char* c)
{
    int dots=0;
    int l_char_type = 0;
    /*
     * l_char_type se vale 0 significa punto, 
     * se vale 1 significa carattere.
     */
    int d_counter=0;
    char* tmp = c;
    if(*tmp == '\0') { return 0; }
    while (*tmp != '\0')
    {
        if(!isdigit(*tmp)){ d_counter=0; }
        if(*tmp == '.')
        {
            if(l_char_type != 0 && dots < 3)
            {
                dots++;
                l_char_type = 0;
            }
            else{ return 0; }
        }
        else if(isdigit(*tmp))
        {
            l_char_type = 1;
            
            if(d_counter < 3){
                /*if(d_counter == 2){
                    char* t = tmp;
                    int n;
                    n = *(t-2)*100;
                    n += *(++t)*10;
                    n += *(++t);
                    if(n > 254){
                        return 0;
                    }
                } NON WORKA E NON HO VOGLIA DI CAPIRE PERCHÉ */
                d_counter++;
            }
            else{ return 0; }
            
        }
        else{ return 0; }

        tmp++;
    }
    return 1;
}

char* recv_nickname(int socket)
{
    int err;
    char *nick = (char*) malloc(sizeof(char) * (NICKNAME_LEN + 1));

    err = recv(socket, nick, NICKNAME_LEN + 1, 0);
    if (err == -1)
        return NULL;
    else
        return nick;
}

int send_nickname(int socket, char* nickname)
{
    int err;

    err = send(socket, nickname, NICKNAME_LEN + 1, 0);
    return err != -1;
} 

/*
char* recv_nickname(int socket)
{
    int len;
    void* buff = malloc(BUFFSIZE);
    gamepkg_t pkg;
    len = recv(socket, buff, BUFFSIZE, 0);
    if (len == -1)
    {
        free(buff);
        return NULL;
    }
    else
    {
        pkg = unpack_pkg(buff, len);
        free(buff);
        if (pkg.type == PKG_NICKNAME)
            return (char*) pkg.data;
        else
        {
            free(pkg.data);
            return NULL;
        }
    }
}

int send_nickname(int socket, char* nickname)
{
    int err;
    gamepkg_t pkg;
    void* buff;
    size_t size = sizeof(ushort) + NICKNAME_LEN + 1;
    pkg.type = PKG_NICKNAME;
    pkg.data = (void*) nickname;
    buff = pack_pkg(pkg, size);
    err = send(socket, buff, size, 0);
    free(buff);
    return err != -1;
}
*/