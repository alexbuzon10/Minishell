#define _GNU_SOURCE

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

void trimNewLine(char *line)
{
    line[strcspn(line, "\n")] = '\0';
}

char *readLine()
{
    char *inpLine = NULL;
    size_t size = 0;
    ssize_t nread = 0;

    nread = getline(&inpLine, &size, stdin);

    if (nread == -1)
    {
        free(inpLine);
        perror("\nError al leer la línea.\n");
        return NULL;
    }

    trimNewLine(inpLine);

    return inpLine;
}