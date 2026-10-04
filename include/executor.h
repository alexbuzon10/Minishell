/********************************************************************
 * FILE:        executor.h
 * AUTHOR:      Alejandro Buzon Garcia 
 * DESCRIPTION: Functions prototypes for executing a command
 ********************************************************************/

#ifndef __MINSHELL_EXECUTOR_H
#define __MINSHELL_EXECUTOR_H

/***************************** Includes *********************************/

#include "minishell.h"

/************************ Functions prototypes **************************/

/**
 * @brief   Executes a command
 */
int execute(cmd_t* cmd);

#endif /* __MINSHELL_PARSER_H */