/************************************************************************
 * FILE:        input.h
 * AUTHOR:      Alejandro Buzon Garcia
 * DESCRIPTION: Function prototypes of input's treatment
 ************************************************************************/

#ifndef __MINSHELL_INPUT_H
#define __MINSHELL_INPUT_H

/******************************* Enums **********************************/

typedef enum escape_state_e {
    ESC_NONE,
    ESC_RECEIVED,
    ESC_CSI,
    ESC_CSI_PARAM,
    ESC_SS3
} escape_state_t;

typedef enum input_status_e {
    INPUT_ERROR = -1,
    INPUT_CONTINUE = 0,
    INPUT_ENTER = 1,
    INPUT_EOF = 2,
    INPUT_NOT_HANDLED = 3,
    INPUT_INTERRUPTED = 4
} input_status_t;

/******************************* Includes *******************************/

#include <termios.h>
#include <stddef.h>

/****************************** Structures ******************************/

typedef struct line_s {
    char    *buffer;
    size_t  size;
    size_t  capacity;
    size_t  cursor_pos;
} line_t;

typedef struct escape_s {
    int     state;
    int     parameter;
} escape_t;

/************************ Functions prototypes **************************/

/**
 * @brief Clean the buffer of the standard input (stdin)
 */
void stdinflush(void);

/**
 * @brief Read a line inputted by the user
 * @return Allocated string or NULL on EOF/error
 */
char *readline(input_status_t *status);

/**
 * @brief Replace '\n' with '\0'
 */
void strtrim(char *line);

/**
 * @brief Replace the first occurrence of a substring in a string
 */
char *replace_first_substring(const char *original, const char *pattern, const char *replacement);

/*************************** Line management ****************************/

/**
 * @brief Initialize an empty line and allocate its initial buffer
 * @return 0 on success, -1 on failure
 */
int init_line(line_t *line);

/**
 * @brief Increase the capacity of the line buffer
 * @return 0 on success, -1 on failure
 */
int expand_line(line_t *line);

/**
 * @brief Free the line buffer and reset its fields
 */
void free_line(line_t *line);

/***************************** Line editing *****************************/

/**
 * @brief Insert a character at the current cursor position
 * @return 0 on success, -1 on failure
 */
int insert_char(line_t *line, char c);

/**
 * @brief Delete the character before the cursor (Backspace)
 */
void delete_before_cursor(line_t *line);

/**
 * @brief Delete the character at the cursor position (Delete)
 */
void delete_at_cursor(line_t *line);

/**************************** Cursor movement ***************************/

/**
 * @brief Move the cursor one position to the left
 */
void move_cursor_left(line_t *line);

/**
 * @brief Move the cursor one position to the right
 */
void move_cursor_right(line_t *line);

/**
 * @brief Move the cursor to the beginning of the line
 */
void move_cursor_home(line_t *line);

/**
 * @brief Move the cursor to the end of the line
 */
void move_cursor_end(line_t *line);

/*************************** Terminal management ************************/

/**
 * @brief Disable canonical mode and terminal echo
 * @param old_term Pointer to store the original terminal attributes
 * @return 0 on success, -1 on failure
 */
int enable_raw_mode(struct termios *old_term);

/**
 * @brief Restore the original terminal attributes
 * @return 0 on success, -1 on failure
 */
int restore_terminal(const struct termios *old_term);

/**
 * @brief Redraw the input line and restore the visual cursor position
 */
void redraw_line(const line_t *line);

/**************************** Input handling ****************************/

/**
 * @brief Initialize the escape sequence parser
 */
void init_escape(escape_t *escape);

/**
 * @brief Process a byte belonging to an ANSI escape sequence
 * @return 1 if consumed, 0 if not consumed
 */
int handle_escape_sequence(line_t *line, escape_t *escape, unsigned char c);

/**
 * @brief Process special control characters
 * @return 0 to continue, 1 on Enter, 2 on EOF, -1 on error
 */
int handle_control_char(line_t *line, char c);

#endif /* __MINSHELL_INPUT_H */