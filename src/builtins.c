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
#include <pwd.h>

#include "../include/minishell.h"

// Builtins table

static int builtinExit(command *cmd) {
    (void)cmd; 

    exit(EXIT_SUCCESS);
}

static int builtinHelp(command *cmd) {
    (void)cmd;

    printf("Lista de comandos: \n");
    printf(" - exit: Salir de la Shell.\n");
    printf(" - help: Muestra información de ayuda.\n");
    printf(" - cd: Cambiar de directorio.\n");
    printf(" - version: Muestra la version de la minishell.\n");

    return SUCCESS;
}

static int builtinCd(command *cmd) {
    if (cmd->argc == 1 || (cmd->argc == 2 && strcmp(cmd->argv[1], "~") == 0)) {
        chdir(getenv("HOME"));
        return SUCCESS;
    } else if (cmd->argc > 2) {
        printf("Uso: cd <ruta_buscad>");
        return SUCCESS;
    }

    int cdStatus = chdir(cmd->argv[1]);

    if (cdStatus == -1) {
        perror("La ruta introducida no existe.\n");
    }

    return SUCCESS;
}

static int builtinVersion(command *cmd) {
    (void) cmd;

    printf("Minishell -- version 0.1.0\n");

    return SUCCESS;
}

static Builtin builtins[] = {
    {"exit", builtinExit},
    {"help", builtinHelp},
    {"cd", builtinCd},
    {"version", builtinVersion}
};

#define NUM_BUILTINS (sizeof(builtins) / sizeof(builtins[0]))

int executeBuiltin(command *cmd) {
    for (size_t i = 0; i < NUM_BUILTINS; i++) {
        if (strcmp(cmd->argv[0], builtins[i].name) == 0) {
            return builtins[i].func(cmd);
        }
    }

    return NOT_A_BUILTIN;
}