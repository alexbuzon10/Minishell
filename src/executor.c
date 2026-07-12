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

int execute(command *cmd) {
    if (executeBuiltin(cmd) == NOT_A_BUILTIN) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork fail");

            return FAIL;
        } else if (pid == 0) {
            execvp(cmd->argv[0], cmd->argv);

            perror(cmd->argv[0]);

            if (errno == ENOENT)
                _exit(127);
        } else {
            int status;

            waitpid(pid, &status, 0);

            int exit_code = WEXITSTATUS(status);

            if (exit_code == 127) {
                printf("El comando introducido no existe.\n");
            }
        }

        return SUCCESS;
    } 

    return -1;
}