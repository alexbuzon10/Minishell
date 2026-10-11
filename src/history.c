/***********************************************************************
 * FILE:        history.c
 * AUTHOR:      Alejandro Buzon Garcia 
 * DESCRIPTION: Command history functions
 ***********************************************************************/

/**************************** Includes *********************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/history.h"

/**************************** Functions ********************************/

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
int init_history(history_t *history) {
    history->capacity = 1;
    history->size = 0;
    history->current = 0;

    history->entries = malloc(sizeof(char *));

    if (history->entries == NULL) {
        perror("malloc");
        return __FUNC_PANIC;
    }

    history->entries[0] = NULL;

    return __FUNC_SUCCESS;
}

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
int save_entry(const char *line, history_t *history) {
    if (history->size == history->capacity) {
        size_t new_capacity = history->capacity * 2;

        char **tmp = realloc(history->entries, new_capacity * sizeof(char *));

        if (tmp == NULL) {
            perror("realloc");
            return __FUNC_FAIL;
        }

        history->entries = tmp;
        history->capacity = new_capacity;
    }

    history->entries[history->size] = strdup(line);

    if (history->entries[history->size] == NULL) {
        perror("strdup");
        return __FUNC_FAIL;
    }

    (history->size)++;
    history->current = history->size;

    return __FUNC_SUCCESS;
}

/**
 * @brief Releases all memory allocated for the command history.
 *
 * Frees each stored command and the entries array, then resets
 * the history structure to an empty state.
 *
 * @param history Pointer to the history structure. If NULL,
 *                the function does nothing.
 */
void free_history(history_t *history) {
    if (history == NULL) return;

    for (size_t i = 0; i < history->size; i++) {
        free(history->entries[i]);
        history->entries[i] = NULL;
    }

    free(history->entries);
    history->entries = NULL;

    history->size = 0;
    history->capacity = 0;
    history->current = 0;
}

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
const char *history_previous(history_t *history) {
    if (history == NULL || history->size == 0)
        return NULL;

    if (history->current > 0) {
        (history->current)--;
    }

    return history->entries[history->current];
}

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
const char *history_next(history_t *history) {
    if (history == NULL || history->size == 0)
        return NULL;

    if (history->current < history->size) {
        history->current++;
    }

    if (history->current == history->size)
        return NULL;

    return history->entries[history->current];
}