/***********************************************************************
 * FILE:        builtins.c
 * AUTHOR:      Alejandro Buzon Garcia 
 * DESCRIPTION: Builtins functions for minishell project
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
#include "../include/input.h"

/**************************** Functions ********************************/

static int buitinExit(const cmd_t *cmd) {
    (void) cmd;
    exit(EXIT_SUCCESS);
}

static int builtinHelp(const cmd_t *cmd) {
    (void) cmd;

    printf("Lista de comandos: \n");
    printf(" - exit: Salir de la Shell.\n");
    printf(" - help: Muestra información de ayuda.\n");
    printf(" - cd: Cambiar de directorio.\n");

    return __FUNC_SUCCESS;
}

static int builtinCd(const cmd_t *cmd) {
    const char *path;
    const char *home;
    char *expanded_path = NULL;
    int result;

    if (cmd == NULL || cmd->argv == NULL)
        return __FUNC_FAIL;

    if (cmd->argc > 2) {
        fprintf(stderr, "cd: demasiados argumentos\n");
        return __FUNC_FAIL;
    }

    home = getenv("HOME");

    if (cmd->argc == 1 || strcmp(cmd->argv[1], "~") == 0) {
        if (home == NULL) {
            fprintf(stderr, "cd: HOME not set\n");
            return __FUNC_FAIL;
        }

        path = home;
    }
    else if (strncmp(cmd->argv[1], "~/", 2) == 0) {
        if (home == NULL) {
            fprintf(stderr, "cd: HOME not set\n");
            return __FUNC_FAIL;
        }

        expanded_path = replace_first_substring(cmd->argv[1], "~", home);

        if (expanded_path == NULL) {
            perror("cd");
            return __FUNC_FAIL;
        }

        path = expanded_path;
    }
    else {
        path = cmd->argv[1];
    }

    result = chdir(path);

    if (result == -1)
        perror("cd");

    free(expanded_path);

    return (result == 0) ? __FUNC_SUCCESS : __FUNC_FAIL;
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
int executebuiltin(const cmd_t *cmd) {
    for (size_t i = 0; i < __NUM_BUILTINS; i++) {
        if (strcmp(cmd->argv[0], builtins[i].name) == 0) {
            return builtins[i].fn(cmd);
        }
    }

    return __NOT_A_BUILTIN;
}