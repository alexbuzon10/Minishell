#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <ctype.h>

#include "../include/minishell.h"

command stringToCommand(char *line) {
    command cmd; 

    cmd.argc = 0;
    cmd.argv = NULL;

    if (line == NULL) return cmd;

    char lineBck[strlen(line) + 1];

    strcpy(lineBck, line);

    char *token = strtok(lineBck, " ");

    while (token != NULL) {
        (cmd.argc)++;

        if (reasignarMemoria(&(cmd.argv), cmd.argc) == 0) {
            // Asigno la memoria del nuevo token

            cmd.argv[cmd.argc - 1] = malloc(strlen(token) + 1 * sizeof(char));

            if (cmd.argv[cmd.argc - 1] == NULL) {
                perror("Error al reservar la memoria del token");
                free_strMemory(&(cmd.argv), cmd.argc);
                cmd.argc = 0;
                break;
            } else {
                strcpy(cmd.argv[cmd.argc - 1], token);
            }
        }

        token = strtok(NULL, " ");
    }

    return cmd;
}

int reasignarMemoria(char ***vec, int size) {
    char **tmp = *vec;

    tmp = realloc(*vec, size * sizeof(char *));

    if (tmp == NULL) {
        perror("Error al reasignar la memoria.\n");
        return -1;
    }

    *vec = tmp;

    return 0;
}

void free_strMemory(char ***vec, int size) {
    if (*vec != NULL) {
        for (int i = 0; i < size; i++)
            free((*vec)[i]);
        free(*vec);
    }
}