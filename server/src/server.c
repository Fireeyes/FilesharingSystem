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

#include "user.h"
#include "command.h"
#include "utils.h"

#define MAX_CLIENTS 10
int fd_max;

user_t *USERS_DB;
int USERS_COUNT = 0;

int quit(int argc, char **argv)
{
    exit(EXIT_SUCCESS);
}

int print(int argc, char **argv)
{
    int i;

    for(i = 0; i < USERS_COUNT; i++) {
        printf("[%d] %s %s\n", i, USERS_DB[i].name, USERS_DB[i].password);
    }

    return 0;
}

command_t command_table[2] = {{"exit", quit}, {"print", print}};
int CMD_COUNT = 2;

int init_server()
{
    int listen_fd, yes = 1, flags = (SO_REUSEADDR | SO_REUSEPORT);
    struct sockaddr_in address;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    DIE(listen_fd < 0, "listen");
    DIE(setsockopt(listen_fd, SOL_SOCKET, flags, &yes, sizeof(int)) < 0, "sockopt");

    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    address.sin_family = AF_INET;

    DIE(bind(listen_fd, (struct sockaddr *)&address, sizeof(address)) < 0, "bind");
    DIE((listen(listen_fd, MAX_CLIENTS) < 0), "listen");

    return listen_fd;
}

int handle_new(int listen_fd)
{
    int new_fd;

    struct sockaddr_in address;
    socklen_t addrlen;

    new_fd = accept(listen_fd, (struct sockaddr *)&address, &addrlen);
    DIE(new_fd < 0, "accept");
    if(new_fd > fd_max) fd_max = new_fd;

    printf("Got new connection from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

    return new_fd;
}

void handle_input()
{
    char *line = read_line();
    int argc; char** argv;
    makeargs(line, &argc, &argv);

    execute_command(argv[0], argc, argv);
    free(line);
}

int main(int argc, char **argv)
{
    char buffer[1024];

    int listen_fd = init_server(), i;
    fd_max = listen_fd;

    fd_set master;
    fd_set read_fds;

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    FD_SET(listen_fd, &master);
    FD_SET(STDIN_FILENO, &master);

    printf("[SERVER] starting\n");
    USERS_DB = read_users("users_config", &USERS_COUNT);

    while(1) {
        read_fds = master;
        DIE(select(fd_max + 1, &read_fds, NULL, NULL, NULL) < 0, "select");

        for(i = 0; i < fd_max + 1; i++) {
            if(FD_ISSET(i, &read_fds)) {
                if(i == listen_fd) {
                    int new_fd = handle_new(listen_fd);
                    FD_SET(new_fd, &master);
                } else if(i == STDIN_FILENO) {
                    handle_input();
                } else {
                    int read_count;

                    if((read_count = read(i, buffer, 1024)) == 0) {
                        printf("Someone disconnected :( \n");
                        FD_CLR(i, &master);
                        close(i);                        
                    } else {
                        printf("Someone sent:%s \n", buffer);
                    }
                }
            }
        }
    }

    printf("[SERVER] stopping\n");
    close(listen_fd);
    return 0;
}
