
#ifndef TERMINAL_CODES_H
#define TERMINAL_CODES_H

/* =========================================
 * ASCII - Caracteres de control
 * ========================================= */

#define ASCII_NUL           0x00
#define ASCII_CTRL_C        0x03
#define ASCII_CTRL_D        0x04
#define ASCII_BELL          0x07
#define ASCII_BACKSPACE     0x08
#define ASCII_TAB           0x09
#define ASCII_NEWLINE       0x0A
#define ASCII_CTRL_L        0x0C
#define ASCII_CARRIAGE_RET  0x0D
#define ASCII_CTRL_Z        0x1A
#define ASCII_ESCAPE        0x1B
#define ASCII_SPACE         0x20
#define ASCII_DELETE        0x7F

/* Secuencias de entrada de las flechas */
#define ASCII_ESC           0x1B
#define ASCII_CSI           0x5B  // '['

#define ASCII_ARROW_UP      0x41  // 'A'
#define ASCII_ARROW_DOWN    0x42  // 'B'
#define ASCII_ARROW_RIGHT   0x43  // 'C'
#define ASCII_ARROW_LEFT    0x44  // 'D'

/* =========================================
 * ANSI - Movimiento del cursor
 * ========================================= */

#define ANSI_CURSOR_UP      "\033[1A"
#define ANSI_CURSOR_DOWN    "\033[1B"
#define ANSI_CURSOR_RIGHT   "\033[1C"
#define ANSI_CURSOR_LEFT    "\033[1D"

#define ANSI_CURSOR_HOME    "\033[H"
#define ANSI_CURSOR_SAVE    "\0337"
#define ANSI_CURSOR_RESTORE "\0338"

/* =========================================
 * ANSI - Borrado de pantalla
 * ========================================= */

#define ANSI_CLEAR_SCREEN       "\033[2J"
#define ANSI_CLEAR_SCREEN_DOWN  "\033[0J"
#define ANSI_CLEAR_SCREEN_UP    "\033[1J"

/* Borra también el historial visible de scrollback
 * en terminales compatibles. */
#define ANSI_CLEAR_SCROLLBACK   "\033[3J"

/* =========================================
 * ANSI - Borrado de línea
 * ========================================= */

#define ANSI_CLEAR_LINE         "\033[2K"
#define ANSI_CLEAR_LINE_RIGHT   "\033[0K"
#define ANSI_CLEAR_LINE_LEFT    "\033[1K"

#define ANSI_LINE_START         "\r"

/* =========================================
 * ANSI - Combinaciones útiles
 * ========================================= */

/* Ir al inicio de la línea y borrarla */
#define ANSI_RESET_LINE         "\r\033[2K"

/* Limpiar pantalla y colocar cursor arriba */
#define ANSI_RESET_SCREEN       "\033[2J\033[H"

/* =========================================
 * ANSI - Apariencia del texto
 * ========================================= */

#define ANSI_STYLE_RESET        "\033[0m"
#define ANSI_BOLD               "\033[1m"
#define ANSI_DIM                "\033[2m"
#define ANSI_UNDERLINE          "\033[4m"
#define ANSI_REVERSE            "\033[7m"

/* =========================================
 * ANSI - Colores de texto
 * ========================================= */

#define ANSI_FG_BLACK           "\033[30m"
#define ANSI_FG_RED             "\033[31m"
#define ANSI_FG_GREEN           "\033[32m"
#define ANSI_FG_YELLOW          "\033[33m"
#define ANSI_FG_BLUE            "\033[34m"
#define ANSI_FG_MAGENTA         "\033[35m"
#define ANSI_FG_CYAN            "\033[36m"
#define ANSI_FG_WHITE           "\033[37m"

/* =========================================
 * ANSI - Colores de fondo
 * ========================================= */

#define ANSI_BG_BLACK           "\033[40m"
#define ANSI_BG_RED             "\033[41m"
#define ANSI_BG_GREEN           "\033[42m"
#define ANSI_BG_YELLOW          "\033[43m"
#define ANSI_BG_BLUE            "\033[44m"
#define ANSI_BG_MAGENTA         "\033[45m"
#define ANSI_BG_CYAN            "\033[46m"
#define ANSI_BG_WHITE           "\033[47m"

/* =========================================
 * Entrada - Secuencias habituales de teclas
 * ========================================= */

#define KEY_ARROW_UP            "\033[A"
#define KEY_ARROW_DOWN          "\033[B"
#define KEY_ARROW_RIGHT         "\033[C"
#define KEY_ARROW_LEFT          "\033[D"

#define KEY_HOME                "\033[H"
#define KEY_END                 "\033[F"
#define KEY_DELETE              "\033[3~"
#define KEY_INSERT              "\033[2~"
#define KEY_PAGE_UP             "\033[5~"
#define KEY_PAGE_DOWN           "\033[6~"

#endif
