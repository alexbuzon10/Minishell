/***********************************************************************
 * FILE:        parser.c
 * AUTHOR:      Alejandro Buzon Garcia 
 * DESCRIPTION: Functions of parser.h
 ***********************************************************************/

/**************************** Includes *********************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "../include/minishell.h"
#include "../include/parser.h"

/**************************** Functions ********************************/

/**
 * @brief Parses a string line and splits it into a command structure.
 */
cmd_t parsecmd(char *line) {
    cmd_t cmd;

    // Set variable with default values
    cmd.argc = 0; 
    cmd.argv = NULL; 

    if (line == NULL) return cmd; // void line -> void cmd !

    char *token = strtok(line, " ");

    while (token != NULL) {
        (cmd.argc)++;

        if (__resize_str_vec(&cmd.argv, cmd.argc) == __FUNC_SUCCESS) {
            cmd.argv[cmd.argc - 1] = malloc(strlen(token) + 1 * sizeof(char)); // I alloc the size of the argument to de vector
            cmd.argv[cmd.argc] = NULL;

            if (cmd.argv[cmd.argc - 1] == NULL) { // null pointer -> FAIL ! :'(
                perror("malloc");
                printf("Errno: %d", errno);
                __free_str_vec(&(cmd.argv), cmd.argc);
                cmd.argc = 0;
                break;
            } else { // not null pointer -> SUCCESS ! :D
                strcpy(cmd.argv[cmd.argc - 1], token);
            }
        } else {
            cmd.argc = 0;
            break;
        }

        token = strtok(NULL, " ");
        
    }

    return cmd;
} 

/**
 * @brief Resizes a dynamic string array (vector).
 */
int __resize_str_vec(char ***str_vec, int size) {
    char **tmp = *str_vec;

    tmp = realloc(tmp, (size + 1) * sizeof(char*)); // 

    if (tmp == NULL) {
        perror("realloc");
        printf("Errno: %d", errno);
        return __FUNC_FAIL;
    }

    *str_vec = tmp;

    return __FUNC_SUCCESS;
}

/**
 * @brief Frees all allocated memory within a string vector.
 */
void __free_str_vec(char ***str_vec, int size) {
    if (*str_vec != NULL) {
        for (int i = 0; i < size; i++) 
            free((*str_vec)[i]);
        free(*str_vec);
    }
    return;
}