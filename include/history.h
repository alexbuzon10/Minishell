/************************************************************************
 * FILE:        history.h
 * AUTHOR:      Alejandro Buzon Garcia 
 * DESCRIPTION: This file contains all functions that are related to 
 *              the command history
 ************************************************************************/

#ifndef __MINSHELL_HISTORY_H
#define __MINSHELL_HISTORY_H

/**************************** Includes *********************************/

#include <stdio.h>

#include "../include/minishell.h"

/**************************** Structures ********************************/

/**
 * @brief Stores the command history
 */
typedef struct history_s {
    char **entries;
    size_t size;
    size_t capacity;
    size_t current;
} history_t;

/************************ Functions prototypes **************************/

/**
 * @brief Initializes the command history.
 *
 * Allocates memory for the history entries array and initializes
 * its size, capacity, and navigation index.
 *
 * @param history Pointer to the history structure to initialize.
 * @return __FUNC_SUCCESS on success, or __FUNC_PANIC if memory
 *         allocation fails.
 */
int init_history(history_t *history);

/**
 * @brief Saves a command in the history.
 *
 * Creates an independent copy of the command and appends it to
 * the history. If the entries array is full, its capacity is
 * doubled. The navigation index is then moved to the end.
 *
 * @param line Null-terminated command string to save.
 * @param history Pointer to the history structure.
 * @return __FUNC_SUCCESS on success, or __FUNC_FAIL if memory
 *         allocation fails.
 */
int save_entry(const char *line, history_t *history);

/**
 * @brief Releases all memory allocated for the command history.
 *
 * Frees each stored command and the entries array, then resets
 * the history structure to an empty state.
 *
 * @param history Pointer to the history structure. If NULL,
 *                the function does nothing.
 */
void free_history(history_t *history);

/**
 * @brief Retrieves the previous command from the history.
 *
 * Moves the navigation index one position backward, if possible,
 * and returns the command at that position.
 *
 * @param history Pointer to the history structure.
 * @return Pointer to the previous command, or NULL if the
 *         history is NULL or empty.
 *
 * @note The returned string belongs to the history structure
 *       and must not be modified or freed by the caller.
 */
const char *history_previous(history_t *history);

/**
 * @brief Retrieves the next command from the history.
 *
 * Moves the navigation index one position forward, if possible.
 * When the index reaches the end of the history, returns NULL
 * to indicate that there is no selected history entry.
 *
 * @param history Pointer to the history structure.
 * @return Pointer to the next command, or NULL if the history
 *         is NULL, empty, or the navigation index reaches
 *         history->size.
 *
 * @note The returned string belongs to the history structure
 *       and must not be modified or freed by the caller.
 */
const char *history_next(history_t *history);

#endif /* __MINSHELL_HISTORY_H */