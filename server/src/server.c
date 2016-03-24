#include<stdio.h>

#include "command.h"

int quit(int argc, char **argv)
{
    exit(EXIT_SUCCESS);
}

command_t command_table[1] = {{"exit", quit}};
int CMD_COUNT = 1;

int main(int argc, char **argv)
{
    printf("[SERVER] starting\n");
    command_interpreter();
    printf("[SERVER] stopping\n");
    return 0;
}
