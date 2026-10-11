/************************************************************************
 * FILE:        signals.h
 * AUTHOR:      Alejandro Buzon Garcia 
 * DESCRIPTION: Signal treatment for Minishell
 ************************************************************************/

#ifndef __MINSHELL_SIGNALS_H
#define __MINSHELL_SIGNALS_H

/***************************** Includes *********************************/

#include <signal.h>
#include <unistd.h>

#include "minishell.h"

/************************ Functions prototypes **************************/

extern volatile sig_atomic_t g_sigint_received; // A flag that getting if SIGINT was received

/**
 * @brief   sigint handler function. It sets g_sigint_received to 1 and
 *          it writes a newline to stdout.
 */
void handler_sigint(int signo);

/**
 * @brief Configure the signal handling for the minishell.
 *
 * @return 0 on success, -1 on failure.
 */
int setup_signals(void);

#endif /* __MINSHELL_SIGNALS_H */