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

/**
 * 
 * @brief Apre la connessione in entrata per 
 * 
 * @return Il socket della connessione con il client.
 */
int host_game()
{
    struct sockaddr_in addr, clt_addr;
    socklen_t addr_len = sizeof(struct sockaddr_in);
    int err;
    int sock, conn_sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        /* Errore */
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    err = bind(sock, (struct sockaddr*) &addr, sizeof(struct sockaddr_in));
    if (err == -1)
    {
        /* Errore */
    }
    
    err = listen(sock, 1);
    if (err == -1)
    {
        /* Errore */
    }

    conn_sock = accept(sock, (struct sockaddr*) &clt_addr, &addr_len);

    if (conn_sock == -1)
    {
        /* Errore */
    }

    close(sock);
    return conn_sock;
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

void* pack_pkg(gamepkg_t pkg)
{
    void* mem = malloc(sizeof(ushort) * 2 + pkg.size);
    memmove(mem, &pkg.type, sizeof(ushort));
    memmove(mem + sizeof(ushort), &pkg.size, sizeof(ushort));
    memmove(mem + (sizeof(ushort) * 2), pkg.data, pkg.size);
    return mem;
}

gamepkg_t unpack_pkg(void* buff)
{
    gamepkg_t pkg;
    memmove(&pkg.type, buff, sizeof(ushort));
    memmove(&pkg.size, buff + sizeof(ushort), sizeof(ushort));
    pkg.data = malloc(pkg.size);
    memmove(pkg.data, buff + (sizeof(ushort) * 2), pkg.size);
    return pkg;
}

int send_pkg(int socket, ushort type, ushort data_size, void* data)
{
    int err;
    gamepkg_t pkg;
    void* buff;
    pkg.type = type;
    pkg.size = data_size;
    pkg.data = data;
    buff = pack_pkg(pkg);
    err = send(socket, buff, data_size + (sizeof(ushort) * 2), 0);
    free(buff);
    return err != -1;
}

gamepkg_t recv_pkg(int socket)
{
    int len;
    void* buff = malloc(BUFFSIZE);
    gamepkg_t pkg;
    len = recv(socket, buff, BUFFSIZE, 0);
    if (len == -1)
    {
        free(buff);
        pkg.type = 0;
        pkg.size = 0;
        pkg.data = NULL;
    }
    else
    {
        pkg = unpack_pkg(buff);
        free(buff);
        return pkg;
    }
    return pkg;
}

char* recv_nickname(int socket)
{
    gamepkg_t pkg = recv_pkg(socket);
    if (pkg.type != PKG_NICKNAME)
    {
        free(pkg.data);
        return NULL;
    }
    else
    {
        return (char*) pkg.data;
    }
}

int* recv_field(int socket)
{
    gamepkg_t pkg = recv_pkg(socket);
    if (pkg.type != PKG_GAMEFIELD)
    {
        free(pkg.data);
        return NULL;
    }
    else
    {
        return (int*) pkg.data;
    }
}

int recv_tet_type(int socket)
{
    gamepkg_t pkg = recv_pkg(socket);
    if (pkg.type != PKG_TET_TYPE)
    {
        free(pkg.data);
        return -1;
    }
    else
    {
        int type = *(int*) pkg.data;
        free(pkg.data);
        return type;
    }
}

int send_nickname(int socket, char* nickname)
{
    ushort size = NICKNAME_LEN + 1;
    return send_pkg(socket, PKG_NICKNAME, size, (void*) nickname);
}

int send_field(int socket, int* field)
{
    ushort size = sizeof(int) * FIELD_ROWS * FIELD_COLS;
    return send_pkg(socket, PKG_GAMEFIELD, size, field);
}

int send_tet_type(int socket, int type)
{
    return send_pkg(socket, PKG_TET_TYPE, sizeof(int), &type);
}

int send_start_game(int socket, int starting)
{
    return send_pkg(socket, PKG_GAME_START, sizeof(int), &starting);
}

int recv_start_game(int socket)
{
    gamepkg_t pkg = recv_pkg(socket);
    if (pkg.type != PKG_GAME_START)
    {
        free(pkg.data);
        return -1;
    }
    else
    {
        int val = *(int*) pkg.data;
        free(pkg.data);
        return val;
    }
}

int send_added_tet(int socket, int added)
{
    return send_pkg(socket, PKG_ADDED, sizeof(int), &added);
}

int recv_added_tet(int socket)
{
    gamepkg_t pkg = recv_pkg(socket);
    if (pkg.type != PKG_ADDED)
    {
        free(pkg.data);
        return -1;
    }
    else
    {
        int val = *(int*) pkg.data;
        free(pkg.data);
        return val;
    }
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
                } NON WORKA E NON HO VOGLIA/TEMPO DI CAPIRE PERCHÉ */
                d_counter++;
            }
            else{ return 0; }
            
        }
        else{ return 0; }

        tmp++;
    }
    return 1;
}
