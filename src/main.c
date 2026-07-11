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

int printPrompt(){
    char *cwd = getcwd(NULL, 0);


    if (cwd != NULL){
        printf("MINISHELL %s> ", cwd);
    } else {
        perror("getcwd error\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int main() {
    char *linea = NULL;

    command cmd;

    while(true) {
        printPrompt();

        linea = readLine();

        if (linea[0] == '\0') {
            continue;
        } else {
            cmd = stringToCommand(linea);

            execute(&cmd);

            if (strcmp(linea, "exit") == 0) {
                free_strMemory(&(cmd.argv), cmd.argc);
                free(linea);
                break;
            }
        }
        
        free_strMemory(&(cmd.argv), cmd.argc);
        free(linea);
    }

    return EXIT_SUCCESS;
}