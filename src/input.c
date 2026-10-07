/***********************************************************************
 * FILE:        input.c
 * AUTHOR:      Alejandro Buzon Garcia 
 * DESCRIPTION: Functions that treat the inputs from the user 
 ***********************************************************************/

/**************************** Includes *********************************/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <errno.h>
#include <termios.h>

#include "../include/input.h"
#include "../include/signals.h"
#include "../include/utils.h"

/**************************** Functions ********************************/

/**
 * @brief Clean the buffer of the standard input (stdin)
 */
void stdinflush(void) {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
    return;
}

/**
 * @brief Replace '\n' to '\0'
 */
void strtrim(char *line) {
    line[strcspn(line, "\n")] = '\0';
    return;
}


/**
 * @brief Read a line inputed by the user
 */
char* readline(void) {
    char* inputline = NULL, *tmp = NULL;
    char c;
    size_t size = 0;

    ssize_t n;

    struct termios oldterm, newterm;

    tcgetattr(STDIN_FILENO, &oldterm); // Save the terminal state

    newterm = oldterm;

    newterm.c_lflag &= ~(ICANON | ECHO);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &newterm);

    while (true) {
        errno = 0;

        n = read(STDIN_FILENO, &c, 1);

        if (n == -1) {
            if (errno == EINTR) {
                if (g_sigint_received) {
                    g_sigint_received = 0;
                    tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldterm);
                    return strdup("");
                }

                continue;
            }

            perror("read");
            free(inputline);
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldterm);
            return NULL;
        } 
        
        if (n > 0 && c == 0x04) {
            if (size == 0){
                tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldterm);
                n = 0;
            } else {
                continue;
            }
        }

        if (n == 0){
            free(inputline);
            tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldterm);
            return NULL;
        }

        if (n > 0) {
            if (c == 0x7f || c == '\b') {
                if (size > 0){
                    size--;
                    inputline[size] = '\0';
                    printf("\r\033[2K");
                    fflush(stdout);
                    redrawline(inputline);
                }
                continue;
            }

            if (c == '\f') {
                clearscreen();
                redrawline(inputline);
                continue;
            }

            if (c == '\n') {
                if (inputline == NULL) {
                    tmp = realloc(inputline, sizeof(char));

                    if (tmp == NULL) {
                        free(inputline);
                        perror("realloc");
                        tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldterm);
                        return NULL;
                    }

                    tmp[0] = '\0';
                    inputline = tmp;
                }
                printf("\n");
                fflush(stdout);
                break;
            }

            size++;

            tmp = realloc(inputline, (size + 1) * sizeof(char));

            if (tmp == NULL) {
                free(inputline);
                perror("realloc");
                tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldterm);
                return NULL;
            }

            tmp[size - 1] = c;
            tmp[size] = '\0';

            inputline = tmp;
        }

        printf("%c", c);
        fflush(stdout);
    }

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldterm);
    return inputline;
}