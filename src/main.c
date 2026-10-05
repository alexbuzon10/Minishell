/***********************************************************************
 * FILE:        main.c
 * AUTHOR:      Alejandro Buzon Garcia 
 * DESCRIPTION: Main program
 ***********************************************************************/

/**************************** Includes *********************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // For using 'bool', 'true' and 'false'
#include <string.h>
#include <signal.h>

#include "../include/minishell.h"
#include "../include/input.h"
#include "../include/executor.h"
#include "../include/parser.h"

/**************************** Functions ********************************/

/**
 * @fn main
 */
int main(void) {
    char* line = NULL;
    cmd_t cmd;

    signal(SIGINT, SIG_IGN);

    while (true) {
        print_prompt();

        line = readline();

        if (line == NULL) break;

        if (line[0] == '\0') {
            free(line);
            continue;
        } else {
            cmd = parsecmd(line);

            execute(&cmd);

            if (strcmp(line, "exit") == 0) {
                __free_str_vec(&(cmd.argv), cmd.argc);
                free(line);
                break;
            }
        }
        __free_str_vec(&cmd.argv, cmd.argc);
        free(line);
    }

    exit(EXIT_SUCCESS);
}