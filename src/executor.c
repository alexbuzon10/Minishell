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
#include "../include/signals.h"
#include "../include/builtins.h"

/**************************** Functions ********************************/

/**
 * @brief   Executes a command
 */
int execute(cmd_t *cmd) {
    if (executebuiltin(cmd) == __NOT_A_BUILTIN){
        signal(SIGINT, SIG_IGN);
        signal(SIGQUIT, SIG_IGN);

        pid_t pid = fork();

        switch (pid) {
            case -1: 
                perror("fork");
                printf("errno=%d", errno);
                return __FUNC_FAIL;
            case 0: 
                signal(SIGINT, SIG_DFL);
                signal(SIGQUIT, SIG_DFL);

                execvp(cmd->argv[0], cmd->argv);
                perror(cmd->argv[0]);
                if (errno == ENOENT)
                    _Exit(127);
                _Exit(126);
            default:
                int status;

                waitpid(pid, &status, 0);

                if (WIFEXITED(status)) {
                    int exit_code = WEXITSTATUS(status);

                    if (exit_code == 127)
                        printf("El comando introducido no existe.\n");
                } else if (WIFSIGNALED(status)) {
                    int sig = WTERMSIG(status);

                    if (sig == SIGINT) {
                        write(STDOUT_FILENO, "\n", 1);
                    }
                    else if (sig == SIGQUIT) {
                        write(STDOUT_FILENO, "Quit\n", 5);
                    }
                }

                signal(SIGINT, handler_sigint);
                signal(SIGQUIT, SIG_IGN);
        }
    }
    return __FUNC_SUCCESS;
}