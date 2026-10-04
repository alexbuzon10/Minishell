/***********************************************************************
 * FILE:        executor.c
 * AUTHOR:      Alejandro Buzon Garcia 
 * DESCRIPTION: Functions of executor.h
 ***********************************************************************/

/**************************** Includes *********************************/

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "../include/minishell.h"
#include "../include/executor.h"

/**************************** Functions ********************************/

/**
 * @brief   Executes a command
 */
int execute(cmd_t* cmd) {
    pid_t pid = fork();

    switch (pid) {
        case -1: 
            perror("fork");
            printf("errno=%d", errno);
            return __FUNC_FAIL;
        case 0: 
            execvp(cmd->argv[0], cmd->argv);
            perror(cmd->argv[0]);
            if (errno == ENOENT)
                _Exit(127);
            _Exit(126);
        default:
            int status;

            waitpid(pid, &status, 0);

            int exit_code = WEXITSTATUS(status);

            if (exit_code == 127)
                printf("El comando introducido no existe.\n");
    }

    return __FUNC_SUCCESS;
}