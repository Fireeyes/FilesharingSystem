#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command.h"

int login(int argc, char **argv)
{
    if(argc < 2) {
        printf("Usage: login <user> <pass>\n");

        return -1;
    }

    return 0;
}

int logout(int argc, char **argv)
{
    printf("Logout\n");

    return 0;
}

int quit(int argc, char **argv)
{
    exit(EXIT_SUCCESS);
}

command_t command_table[3] = {{"login", login}, {"logout", logout}, {"exit", quit}};
int CMD_COUNT = 3;

int main(int argc, char **argv)
{
    printf("[CLIENT] starting\n");
    command_interpreter();
    printf("[CLIENT] stopping\n");
    return 0;
}
