/***********************************************************************
 * FILE:        utils.c
 * AUTHOR:      Alejandro Buzon Garcia 
 * DESCRIPTION: This file defines the utilsfunctions
 ***********************************************************************/

/**************************** Includes *********************************/

#include <stdio.h>
#include <unistd.h>

#include "../include/utils.h"
#include "../include/minishell.h"
#include "../include/terminal_codes.h"

/**************************** Functions ********************************/

void clearscreen(void) {
    write(STDOUT_FILENO, ANSI_RESET_SCREEN, 7);
    return;
}

void redrawline(const char *inputline){
    print_prompt();
    fflush(stdout);
    if (inputline != NULL)
        printf("%s", inputline);
    fflush(stdout);
    return;
}