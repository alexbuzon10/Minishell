/***********************************************************************
 * FILE:        prompt.c
 * AUTHOR:      Alejandro Buzon Garcia 
 * DESCRIPTION: Treats the prompt
 ***********************************************************************/

/**************************** Includes *********************************/

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "../include/minishell.h"

/**************************** Functions ********************************/

/**
 * NOTE: this fuction shall be in a .h 
 */
void replace_diff_length(char *str, const char *old_sub, const char *new_sub) {
    char *pos = strstr(str, old_sub);
    if (pos) {
        size_t old_len = strlen(old_sub);
        size_t new_len = strlen(new_sub);
        size_t tail_len = strlen(pos + old_len);

        // Shift tail left or right to accommodate new length
        memmove(pos + new_len, pos + old_len, tail_len + 1);
        
        // Copy new substring
        memcpy(pos, new_sub, new_len);
    }
}   

/**
 * @brief A function that prints the prompt
 */
int print_prompt(void){
    char *cwd = getcwd(NULL, 0);

    if (cwd != NULL){
        replace_diff_length(cwd, getenv("HOME"), "~");
        printf("MINISHELL %s $ ", cwd);
    } else {
        perror("getcwd error\n");
        free(cwd);
        exit(EXIT_FAILURE);
    }

    free(cwd);
    return 0;
}