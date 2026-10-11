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
    write(STDOUT_FILENO, "^C", 2);
}

/**
 * @brief Configure the signal handling for the minishell.
 *
 * @return 0 on success, -1 on failure.
 */
int setup_signals(void) {
    struct sigaction sa;

    sa.sa_handler = handler_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(SIGINT, &sa, NULL) == -1) {
        perror("sigaction");
        return __FUNC_PANIC;
    }

    return __FUNC_SUCCESS;
}