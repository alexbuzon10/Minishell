/************************************************************************
 * FILE:        minishell.h
 * AUTHOR:      Alejandro Buzon Garcia 
 * DESCRIPTION: Global definitions, structures, and function prototypes 
 *              for the Minishell CLI project.
 ************************************************************************/


#ifndef __MINSHELL_MINISHELL_H
#define __MINSHELL_MINISHELL_H

/**************************** Includes *********************************/

#include <stdlib.h>
#include <stdio.h>

/**************************** Defines ***********************************/

#define __FUNC_FAIL 1
#define __FUNC_SUCCESS 0
#define __NOT_A_BUILTIN -1

/**************************** Structures ********************************/

/**
 * @brief Stores the arguments of a parsed command
 */
typedef struct cmd_s {
    int argc; 
    char **argv;
} cmd_t;

/************************ Functions prototypes **************************/

/**
 * @brief A function that prints the prompt
 */
int print_prompt(void);

/**
 * @brief   Resizes a dynamic string array (vector)
 */
int __resize_str_vec(char*** str_vec, int size);

/**
 * @brief   Free the memmory of a dynamic string array (vector)
 */
void __free_str_vec(char*** str_vec, int size);

#endif /* __MINSHELL_MINISHELL_H */