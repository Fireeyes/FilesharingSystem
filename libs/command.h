#ifndef _COMMAND_H_
#define _COMMAND_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define CHUNK_SIZE 100
#define ERR_ALLOCATION  "Unable to allocate memory"
#define ERR_INVALID_CMD "Command not found"

typedef struct command {
    char *name;
    int (*func)(int, char **);
} command_t;

extern int CMD_COUNT;
extern command_t command_table[];

/**
 * Readline from stdin
 * Took from SO assignment
 */
char *read_line()
{
	char *instr;
	char *chunk;
	char *ret;

	int instr_length;
	int chunk_length;

	int endline = 0;

	instr = NULL;
	instr_length = 0;

	chunk = calloc(CHUNK_SIZE, sizeof(char));
	if (chunk == NULL) {
		fprintf(stderr, ERR_ALLOCATION);
		return instr;
	}

	while (!endline) {
		ret = fgets(chunk, CHUNK_SIZE, stdin);
		if (ret == NULL)
			break;

		chunk_length = strlen(chunk);
		if (chunk[chunk_length - 1] == '\n') {
			chunk[chunk_length - 1] = 0;
			endline = 1;
		}

		instr = realloc(instr, instr_length + CHUNK_SIZE + 1);
		if (instr == NULL)
			break;

		memset(instr + instr_length, 0, CHUNK_SIZE);
		strcat(instr, chunk);
		instr_length += chunk_length;
	}

	free(chunk);

	return instr;
}

int makeargs(char *args, int *argc, char ***aa) {
    int c = 1;
    char *delim;
    char **argv = calloc(c, sizeof (char *));

    argv[0] = args;

    while ((delim = strchr(argv[c - 1], ' '))) {
        argv = realloc(argv, (c + 1) * sizeof (char *));
        argv[c] = delim + 1;
        *delim = 0x00;
        c++;
    }

    *argc = c;
    *aa = argv;

    return c;
}


int execute_command(char *name, int argc, char **argv)
{
    int i = 0;

    for (i = 0; i < CMD_COUNT; i++) {
        if(strcmp(name, command_table[i].name) == 0) {
            return command_table[i].func(argc, argv);
        }
    }

    printf("Invalid command\n");
    return -100;
}

void command_interpreter()
{
    char *line;

    while(1) {
        printf("> ");
        fflush(stdout);

        line = read_line();
        int argc; char** argv;
        makeargs(line, &argc, &argv);

        execute_command(argv[0], argc, argv);
        free(line);
    }
}

#endif
