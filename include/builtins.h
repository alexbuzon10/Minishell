/********************************************************************
 * FILE:        buitins.h
 * AUTHOR:      Alejandro Buzon Garcia 
 * DESCRIPTION: Customs buitins for minishell
 ********************************************************************/

#ifndef __MINSHELL_BUILTINS_H
#define __MINSHELL_BUILTINS_H

/***************************** Includes *********************************/

#include "minishell.h"

/**************************** Structures ********************************/

/**
 * @brief Builtin's function
 */
typedef int (*builtinfunc)(cmd_t*);

/**
 * @brief Stores a builtin
 */
typedef struct builtin_s {
    const char* name; 
    builtinfunc fn; 
} builtin_t;

/************************ Functions prototypes **************************/

/**
 * @brief   Executes a builtin
 */
int executebuiltin(cmd_t* cmd); 

#endif /* __MINSHELL_BUILTINS_H */