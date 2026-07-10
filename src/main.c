#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <ctype.h>

#include "../include/minishell.h"

int main()
{
    char *prompt = "minishell>  ";
    char *linea = NULL;

    command cmd1;

    while(true)
    {
        printf("%s", prompt);

        linea = readLine();

        if (linea == NULL) break;

        cmd1 = stringToCommand(linea);

        printf("El comando introducido es: <%s>\n", linea);
        printf("Número de argumentos: %d\n", cmd1.argc);

        for (int i = 0; i < cmd1.argc; i++)
        {
            printf("Argumento %d: <%s>\n", i + 1, cmd1.argv[i]);
        }

        if (strcmp(linea, "exit") == 0)
        {
            free(linea);
            break;
        }

        free(linea);
    }
}