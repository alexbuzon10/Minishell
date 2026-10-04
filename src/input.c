/***********************************************************************
 * FILE:        input.c
 * AUTHOR:      Alejandro Buzon Garcia 
 * DESCRIPTION: Functions that treat the inputs from the user 
 ***********************************************************************/

/**************************** Includes *********************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/input.h"

/**************************** Functions ********************************/

/**
 * @brief Clean the buffer of the standard input (stdin)
 */
void stdinflush(void) {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
    return;
}

/**
 * @brief Replace '\n' to '\0'
 */
void strtrim(char* line) {
    line[strcspn(line, "\n")] = '\0';
    return;
}


/**
 * @brief Read a line inputed by the user
 */
char* readline(void) {
    char* inputline = NULL;
    size_t size = 0;
    ssize_t nread = 0;

    nread = getline(&inputline, &size, stdin);

    if (nread == -1) {
        free(inputline);
        perror("\nError al leer la línea.\n");
        return NULL;
    }

    strtrim(inputline);

    return inputline;
}