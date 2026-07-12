/**********************************************
 * 
 *               MINISHELL.H
 * 
 *********************************************/

#ifndef MINISHELL_H
#define MINISHELL_H

#define NOT_A_BUILTIN -1
#define FAIL -1
#define SUCCESS 0

typedef struct {
    char **argv;
    int argc;
} command;

typedef int (*BuiltinFunc)(command *);

typedef struct {
    const char * name;
    BuiltinFunc func;

} Builtin;

//// input.c

void trimNewLine(char *line);

char *readLine();

//// parser.c

command stringToCommand(char *line);

int reasignarMemoria(char ***vec, int size);

void free_strMemory(char ***vec, int size);

//// executor.c

int execute(command *cmd);

//// builtins.c

int executeBuiltin(command *cmd);

//// utils.c



#endif 