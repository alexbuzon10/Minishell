/***********************************************************************
 * FILE:        signals.c
 * AUTHOR:      Alejandro Buzon Garcia 
 * DESCRIPTION: This file defines the signal-handling functions
 ***********************************************************************/

/**************************** Includes *********************************/

#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "../include/minishell.h"
#include "../include/signals.h"

volatile sig_atomic_t g_sigint_received = 0; // This flag is set to 1 in case of receiving SIGINT (1)

/**************************** Functions ********************************/

/**
 * @brief   sigint handler function. It sets g_sigint_received to 1 and
 *          it writes a newline to stdout.
 */
void handler_sigint(int signo){
    (void) signo;

    g_sigint_received = 1; // (1) Like this
    write(STDOUT_FILENO, "\n", 1);
    fflush(stdout);
}