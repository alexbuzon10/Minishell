/**********************************************
 * 
 *               MINISHELL.H
 * 
 *********************************************/

typedef struct
{
    char **argv;
    int argc;
} command;

//// input.c

void trimNewLine(char *line);

char *readLine();

//// parser.c

command stringToCommand(char *line);

int reasignarMemoria(char ***vec, int size);

void free_strMemory(char ***vec, int size);