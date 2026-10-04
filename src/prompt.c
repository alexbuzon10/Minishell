/***********************************************************************
 * FILE:        prompt.c
 * AUTHOR:      Alejandro Buzon Garcia 
 * DESCRIPTION: Treats the prompt
 ***********************************************************************/

/**************************** Includes *********************************/

#include <stdlib.h>
#include <unistd.h>

#include "../include/minishell.h"

/**************************** Functions ********************************/

/**
 * @brief A function that prints the prompt
 */
int print_prompt(void){
    char *cwd = getcwd(NULL, 0);

    if (cwd != NULL){
        printf("MINISHELL %s> ", cwd);
    } else {
        perror("getcwd error\n");
        free(cwd);
        exit(EXIT_FAILURE);
    }

    free(cwd);
    return 0;
}