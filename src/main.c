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
#include "../include/signals.h"
#include "../include/utils.h"
#include "../include/history.h"

/**************************** Functions ********************************/

/**
 * @fn main
 */
int main(void) {
    char* line = NULL;
    cmd_t cmd;
    input_status_t status;
    history_t history; 
    bool running = true;

    if (init_history(&history) < 0) {
        perror("init_history");
        return EXIT_FAILURE;
    }

    if (setup_signals() != __FUNC_SUCCESS) return EXIT_FAILURE;

    while (running) {
        // print_prompt();
        // fflush(stdout);

        line = readline(&status, &history);

        switch (status) {
            case INPUT_ERROR:
            case INPUT_EOF:
                running = false;
                break;

            case INPUT_INTERRUPTED:
                continue;

            case INPUT_ENTER:
                break;

            case INPUT_CONTINUE:
            case INPUT_NOT_HANDLED:
                /* Estos estados no deberían llegar a main() */
                running = false;
                break;
        }

        if (line == NULL) break;

        if (line[0] == '\0') {
            free(line);
            continue;
        } else {
            if (save_entry(line, &history) != __FUNC_SUCCESS) {
                free(line);
                break;
            }

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