/***********************************************************************
 * FILE:        main.c
 * AUTHOR:      Alejandro Buzon Garcia 
 * DESCRIPTION: Main program
 ***********************************************************************/

/**************************** Includes *********************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>
#include <fcntl.h>

#include "../include/builtins.h"
#include "../include/minishell.h"

/**************************** Functions ********************************/

static int buitinExit(cmd_t* cmd) {
    (void) cmd;
    exit(EXIT_SUCCESS);
}

static int builtinHelp(cmd_t* cmd) {
    (void) cmd;

    printf("Lista de comandos: \n");
    printf(" - exit: Salir de la Shell.\n");
    printf(" - help: Muestra información de ayuda.\n");
    printf(" - cd: Cambiar de directorio.\n");

    return __FUNC_SUCCESS;
}

static int builtinCd(cmd_t* cmd) {
    if (cmd->argc == 1 || (cmd->argc == 2 && strcmp(cmd->argv[1], "~") == 0)) {
        chdir(getenv("HOME"));
        return 0;
    } else if (cmd->argc > 2) {
        printf("Uso: cd <ruta_buscada>");
        return 0;
    }

    chdir(cmd->argv[1]);

    if (errno == __O_DIRECTORY) {
        perror("La ruta introducida no existe.\n");
        return __FUNC_FAIL;
    }

    return __FUNC_SUCCESS;
}

static builtin_t builtins[] = {
    {"exit", buitinExit},
    {"cd", builtinCd},
    {"help", builtinHelp}
};

#define __NUM_BUILTINS (sizeof(builtins)/sizeof(builtin_t)) // I define the numbers of buitins.

/**
 * @brief   Executes a builtin
 */
int executebuiltin(cmd_t* cmd) {
    for (size_t i = 0; i < __NUM_BUILTINS; i++) {
        if (strcmp(cmd->argv[0], builtins[i].name) == 0) {
            return builtins[i].fn(cmd);
        }
    }

    return __NOT_A_BUILTIN;
}

