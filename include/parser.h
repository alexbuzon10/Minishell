/********************************************************************
 * FILE:        parser.h
 * AUTHOR:      Alejandro Buzon Garcia 
 * DESCRIPTION: Functions prototypes for parsing strings to commands
 ********************************************************************/

#ifndef __MINSHELL_PARSER_H
#define __MINSHELL_PARSER_H

/***************************** Includes *********************************/

#include "minishell.h"

/************************ Functions prototypes **************************/

/**
 * @brief   Parses a string to a cmd_t structure
 */
cmd_t parsecmd(char* line); 

#endif /* __MINSHELL_PARSER_H */