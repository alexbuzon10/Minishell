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
#include "../include/terminal_codes.h"
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
 * @brief Read a line inputted by the user
 * @return Allocated string or NULL on EOF/error/interruption
 */
char *readline(input_status_t *status) {
    line_t line = {0};
    escape_t escape;
    struct termios old_term;
    unsigned char c;
    ssize_t n;
    char *result = NULL;

    if (status == NULL) return NULL;

    *status = INPUT_ERROR;

    if (init_line(&line) != __FUNC_SUCCESS)
        goto cleanup;

    init_escape(&escape);

    if (enable_raw_mode(&old_term) == -1)
        goto cleanup;

    g_sigint_received = 0;

    print_prompt();
    fflush(stdout);

    while (1) {
        if (g_sigint_received) {
            g_sigint_received = 0;
            putchar('\n');
            goto restore;
        }

        n = read(STDIN_FILENO, &c, 1);

        if (n == -1) {
            if (errno == EINTR) {
                if (g_sigint_received) {
                    *status = INPUT_INTERRUPTED;
                    g_sigint_received = 0;
                    putchar('\n');
                    goto restore;
                }

                continue;
            }

            perror("read");
            goto restore;
        }

        if (n == 0)
            goto restore;

        if (handle_escape_sequence(&line, &escape, c))
            continue;

        *status = handle_control_char(&line, (char)c);

        if (*status == INPUT_ENTER) {
            putchar('\n');
            break;
        }

        if (*status == INPUT_EOF || *status == INPUT_ERROR)
            goto restore;

        if (*status == INPUT_CONTINUE)
            continue;

        if (*status == INPUT_NOT_HANDLED) {
            if (insert_char(&line, (char)c) != __FUNC_SUCCESS)
                goto restore;

            redraw_line(&line);
        }
    }

    result = line.buffer;
    line.buffer = NULL;

restore:
    if (restore_terminal(&old_term) == -1) {
        perror("restore_terminal");
        free(result);
        result = NULL;
    }

cleanup:
    free_line(&line);
    return result;
}

/**
 * @brief Replace '\n' to '\0'
 */
void strtrim(char *line) {
    line[strcspn(line, "\n")] = '\0';
    return;
}

/**
 * @brief Replace the first occurrence of a substring in a string
 */
char *replace_first_substring(const char *original, const char *pattern, const char *replacement) {
    const char *found = strstr(original, pattern);
    if (!found) return strdup(original); // no coincidence

    size_t patlen = strlen(pattern);
    size_t repllen = strlen(replacement);
    size_t prefix_len = found - original;
    size_t suffix_len = strlen(found + patlen);
    size_t newlen = prefix_len + repllen + suffix_len;

    char *result = malloc(newlen + 1);
    if (!result) return NULL;

    memcpy(result, original, prefix_len);
    memcpy(result + prefix_len, replacement, repllen);
    memcpy(result + prefix_len + repllen, found + patlen, suffix_len + 1);

    return result;
}

/*************************** Line management ****************************/

/**
 * @brief Initialize an empty line and allocate its initial buffer
 * @return 0 on success, -1 on failure
 */
int init_line(line_t *line) {
    line->size = 0; 
    line->capacity = 1;
    line->cursor_pos = 0;
    line->buffer = malloc(line->capacity * sizeof(char));

    if (line->buffer == NULL) {
        perror("malloc");
        line->capacity = 0;
        return __FUNC_PANIC;
    }

    line->buffer[0] = '\0';

    return __FUNC_SUCCESS;
}

/**
 * @brief Increase the capacity of the line buffer
 * @return 0 on success, -1 on failure
 */
int expand_line(line_t *line) {
    char *tmp = NULL;
    
    (line->capacity)++;

    tmp = realloc(line->buffer, line->capacity * sizeof(char));

    if (tmp == NULL) {
        perror("realloc");
        (line->capacity)--;
        return __FUNC_PANIC;
    }

    line->buffer = tmp;

    return __FUNC_SUCCESS;
}

/**
 * @brief Free the line buffer and reset its fields
 */
void free_line(line_t *line) {
    if (line->buffer == NULL) return;

    free(line->buffer);

    line->buffer = NULL;
    line->capacity = 0;
    line->size = 0;
    line->cursor_pos = 0;
}

/***************************** Line editing *****************************/

/**
 * @brief Insert a character at the current cursor position
 * @return 0 on success, -1 on failure
 */
int insert_char(line_t *line, char c) {
    if (line == NULL) {
        return __FUNC_PANIC;
    }

    if (line->buffer == NULL) {
        if (init_line(line) == __FUNC_PANIC) {
            return __FUNC_PANIC;
        }
    }

    if (line->cursor_pos > line->size) {
        return __FUNC_PANIC;
    }

    if (line->size >= line->capacity - 1) {
        if (expand_line(line) == __FUNC_PANIC) {
            return __FUNC_PANIC;
        }
    }

    memmove(line->buffer + line->cursor_pos + 1, line->buffer + line->cursor_pos, line->size - line->cursor_pos + 1);

    line->buffer[line->cursor_pos] = c;

    line->size++;
    line->cursor_pos++;

    return __FUNC_SUCCESS;
}

/**
 * @brief Delete the character before the cursor (Backspace)
 */
void delete_before_cursor(line_t *line) {
    if (line == NULL || line->buffer == NULL || line->cursor_pos == 0) {
        return;
    }

    if (line->cursor_pos == line->size) {
        (line->cursor_pos)--, (line->size)--;
    } else {
        (line->cursor_pos)--, (line->size)--;
        memmove(line->buffer + line->cursor_pos, line->buffer + line->cursor_pos + 1, line->size - line->cursor_pos + 1);
    }

    line->buffer[line->size] = '\0';
}

/**
 * @brief Delete the character at the cursor position (Delete)
 */
void delete_at_cursor(line_t *line) {
    if (line == NULL || line->buffer == NULL || line->cursor_pos == line->size) {
        return;
    }

    memmove(line->buffer + line->cursor_pos, line->buffer + line->cursor_pos + 1, line->size - line->cursor_pos);

    (line->size)--;
}

/**************************** Cursor movement ***************************/

/**
 * @brief Move the cursor one position to the left
 */
void move_cursor_left(line_t *line) {
    if (line == NULL || line->cursor_pos == 0) return;
    (line->cursor_pos)--;
    printf(ANSI_CURSOR_LEFT);
    fflush(stdout);
}

/**
 * @brief Move the cursor one position to the right
 */
void move_cursor_right(line_t *line) {
    if (line == NULL || line->cursor_pos == line->size) return;
    (line->cursor_pos)++;
    printf(ANSI_CURSOR_RIGHT);
    fflush(stdout);
}

/**
 * @brief Move the cursor to the beginning of the line
 */
void move_cursor_home(line_t *line) {
    if (line == NULL || line->cursor_pos == 0) return;
    printf("\033[%zuD", line->cursor_pos);
    fflush(stdout);
    line->cursor_pos = 0;
}

/**
 * @brief Move the cursor to the end of the line
 */
void move_cursor_end(line_t *line) {
    if (line == NULL || line->cursor_pos == line->size) return;
    printf("\033[%zuC", line->size - line->cursor_pos);
    fflush(stdout);
    line->cursor_pos = line->size;
}

/**
 * @brief Disable canonical mode and terminal echo
 * @param old_term Pointer to store the original terminal attributes
 * @return 0 on success, -1 on failure
 */
int enable_raw_mode(struct termios *old_term) {
    if (old_term == NULL) return __FUNC_PANIC;

    if (tcgetattr(STDIN_FILENO, old_term) == -1) return __FUNC_PANIC;

    struct termios raw = *old_term;

    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) return __FUNC_PANIC;

    return __FUNC_SUCCESS;
}

/**
 * @brief Restore the original terminal attributes
 * @return 0 on success, -1 on failure
 */
int restore_terminal(const struct termios *old_term) {
    if (old_term == NULL) return __FUNC_PANIC;
    return tcsetattr(STDIN_FILENO, TCSANOW, old_term);
}

/**
 * @brief Redraw the input line and restore the visual cursor position
 */
void redraw_line(const line_t *line) {
    if (line == NULL) return;
    printf(ANSI_LINE_START);
    printf(ANSI_CLEAR_LINE);
    print_prompt();
    if (line->buffer != NULL) {
        printf("%s", line->buffer);

        if (line->cursor_pos <= line->size) {
            size_t offset = line->size - line->cursor_pos;

            if (offset > 0) printf("\033[%zuD", offset);
        }
    }

    fflush(stdout);
}

/**
 * @brief Initialize the escape sequence parser
 */
void init_escape(escape_t *escape) {
    if (escape == NULL) return;
    escape->state = ESC_NONE;
    escape->parameter = 0;
}

/**
 * @brief Process a byte belonging to an ANSI escape sequence
 * @return 1 if consumed, 0 if not consumed
 */
int handle_escape_sequence(line_t *line, escape_t *escape, unsigned char c) {
    if (line == NULL || escape == NULL)
        return 0;

    if (escape->state == ESC_NONE) {
        if (c == 0x1B) {
            escape->state = ESC_RECEIVED;
            return 1;
        }

        return 0;
    }

    if (escape->state == ESC_RECEIVED) {
        if (c == '[') {
            escape->state = ESC_CSI;
            escape->parameter = 0;
        } else {
            escape->state = ESC_NONE;
        }

        return 1;
    }

    if (escape->state == ESC_CSI) {
        if (c == 'C') {
            escape->state = ESC_NONE;
            move_cursor_right(line);
        } else if (c == 'D') {
            escape->state = ESC_NONE;
            move_cursor_left(line);
        } else if (c == 'H') {
            escape->state = ESC_NONE;
            move_cursor_home(line);
        } else if (c == 'F') {
            escape->state = ESC_NONE;
            move_cursor_end(line);
        } else if (c >= '0' && c <= '9') {
            escape->parameter = c - '0';
            escape->state = ESC_CSI_PARAM;
        } else {
            escape->state = ESC_NONE;
        }

        return 1;
    }

    if (escape->state == ESC_CSI_PARAM) {
        if (c >= '0' && c <= '9') {
            if (escape->parameter < 1000) escape->parameter = escape->parameter * 10 + (c - '0');

            return 1;
        }

        if (c == '~') {
            if (escape->parameter == 3) {
                delete_at_cursor(line);
                redraw_line(line);
            } else if (escape->parameter == 1 || escape->parameter == 7) {
                move_cursor_home(line);
            } else if (escape->parameter == 4 || escape->parameter == 8) {
                move_cursor_end(line);
            }
        }

        escape->state = ESC_NONE;
        escape->parameter = 0;

        return 1;
    }

    escape->state = ESC_NONE;
    escape->parameter = 0;
    return 1;
}

/**
 * @brief Handle control characters received from stdin
 * @return Input status indicating the action to perform
 */
int handle_control_char(line_t *line, char c) {
    if (line == NULL)
        return INPUT_ERROR;

    /* Enter */
    if (c == '\n' || c == '\r') {
        return INPUT_ENTER;
    }

    /* Ctrl+D */
    if (c == 0x04) {
        if (line->size == 0)
            return INPUT_EOF;

        return INPUT_CONTINUE;
    }

    /* Backspace */
    if (c == 0x7F || c == 0x08) {
        delete_before_cursor(line);
        redraw_line(line);
        return INPUT_CONTINUE;
    }

    /* Ctrl+L */
    if (c == 0x0C) {
        printf("\033[2J\033[H");
        redraw_line(line);
        return INPUT_CONTINUE;
    }

    /* Ctrl+A */
    if (c == 0x01) {
        move_cursor_home(line);
        return INPUT_CONTINUE;
    }

    /* Ctrl+E */
    if (c == 0x05) {
        move_cursor_end(line);
        return INPUT_CONTINUE;
    }

    /* Ignore unsupported control characters */
    if ((unsigned char)c < 0x20 || (unsigned char)c == 0x7F) {
        return INPUT_CONTINUE;
    }

    return INPUT_NOT_HANDLED;
}