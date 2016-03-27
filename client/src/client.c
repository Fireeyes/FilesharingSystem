#include <stdio.h>
#include <string.h>   //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>   //close
#include <arpa/inet.h>    //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <arpa/inet.h>

#include "networking.h"
#include "command.h"
#include "utils.h"

int server; // server socket

int login(int argc, char **argv)
{
    if(argc < 2) {
        printf("Usage: login <user> <pass>\n");

        return -1;
    }
    
    packet_t pck;
    pck.length = strlen(argv[argc]) * sizeof(char) + sizeof(int) + sizeof(packet_type);
    pck.type = REQUEST;
    strcpy(pck.data, argv[argc]);
    
    printf("Sending [%d] - %s\n", pck.length, pck.data);
    send(server, &pck, pck.length, 0);
    
    return 0;
}

int logout(int argc, char **argv)
{
    printf("Logout\n");

    return 0;
}

int quit(int argc, char **argv)
{
    close(server);
    exit(EXIT_SUCCESS);
}

command_t command_table[3] = {{"login", login}, {"logout", logout}, {"exit", quit}};
int CMD_COUNT = 3;

int init_connection(char *ip)
{
    int server_fd;
    struct sockaddr_in server;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    DIE(server_fd < 0, "socket");
    memset(&server, '0', sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    DIE(inet_pton(AF_INET, ip, &server.sin_addr) < 0, "inet_pton");
    DIE(connect(server_fd, (struct sockaddr*)&server, sizeof(server)) < 0, "connect" );

    return server_fd;
}

int main(int argc, char **argv)
{
    if(argc < 2) {
        printf("usage %s <ip>\n", argv[0]);
        exit(0);
    }
    server = init_connection(argv[1]);

    printf("[CLIENT] starting\n");
    command_interpreter();
    printf("[CLIENT] stopping\n");
    return 0;
}
