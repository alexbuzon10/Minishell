/************************************************************************
 * FILE:        input.h
 * AUTHOR:      Alejandro Buzon Garcia 
 * DESCRIPTION: Function prototypes of input's treatment
 ************************************************************************/


#ifndef __MINSHELL_INPUT_H
#define __MINSHELL_INPUT_H

/************************ Functions prototypes **************************/

/**
 * @brief Clean the buffer of the standard input (stdin)
 */
void stdinflush(void);

/**
 * @brief Read a line inputed by the user
 */
char* readline(void);

/**
 * @brief Read a line inputed by the user
 */
char* readline(void);

/**
 * @brief Replace '\n' to '\0'
 */
void strtrim(char *line);

#endif /* __MINSHELL_INPUT_H */