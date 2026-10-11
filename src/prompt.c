/***********************************************************************
 * FILE:        prompt.c
 * AUTHOR:      Alejandro Buzon Garcia 
 * DESCRIPTION: Treats the prompt
 ***********************************************************************/

/**************************** Includes *********************************/

#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>

#include "../include/minishell.h"
#include "../include/terminal_codes.h"
#include "../include/input.h"

/**************************** Functions ********************************/

#define MAX_SIZE 250

/**
 * @brief A function that prints the prompt
 */
int print_prompt(void){
    char *cwd = getcwd(NULL, 0);
    const char *home = getenv("HOME");

    char hostname[MAX_SIZE];
    if (gethostname(hostname, MAX_SIZE) < 0) {
        perror("gethostname");
        return __FUNC_FAIL;
    }

    struct passwd *pw = getpwuid(getuid());

    if (pw == NULL) {
        perror("getpwuid");
        return __FUNC_FAIL;
    }

    if (cwd != NULL){

        if (home != NULL && home[0] != '\0' && strstr(cwd, home) == cwd && (cwd[strlen(home)] == '/' || cwd[strlen(home)] == '\0')) {
            char *new_cwd = replace_first_substring(cwd, home, "~");

            if (new_cwd != NULL) {
                free(cwd);
                cwd = new_cwd;
            }
        }
        if (strcmp(pw->pw_name, "root") == 0) printf(ANSI_FG_RED "%s@%s" ANSI_STYLE_RESET ":" ANSI_FG_BLUE "%s" ANSI_STYLE_RESET "# ", pw->pw_name, hostname, cwd);
        else printf(ANSI_FG_GREEN "%s@%s" ANSI_STYLE_RESET ":" ANSI_FG_BLUE "%s" ANSI_STYLE_RESET "$ ", pw->pw_name, hostname, cwd);
    } else {
        perror("getcwd error\n");
        free(cwd);
        return __FUNC_FAIL;
    }

    free(cwd);
    return __FUNC_SUCCESS;
}