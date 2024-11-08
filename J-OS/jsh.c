/*
LillyDevStudios LLC. 2024 ©


JSH is a BASIC system shell for use in J-OS(Linux Implementation Project) with predefined 
J-Specific commands and utilities.

it is NOT recommended to change these lines of code before reading README-JSH-SRC.md.
Please read README-JSH-SRC.md before changing anything on this code.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define dev "DEVELOPER: LillyDeVT"
#define version "JSH VERSION: 0.1"

#define HLPMSG "Type hlp to show this message again\n\nType %s'dev'%s to show the shell version and developer\n\nType %sOSINFO%s for some useful OS Information!\n\ntype any other %sUNIX%s command to use it.\n\ndon't expect any other new fatures, this is just another SH Environment.\n\n"

#define PROMPT "| %s:%s%s%s [%s%s%s] |>> "

/*
For States we will do different ones.
- RCLR = Not a ERR or a WARN, continue.
- WARN   = Needs attention, but continue.
- ERR    = NEEDS IMMEDIATE ATTENTION, HALT.

- DIR  : SYSTEM
- HOST : SYSTEM
- USER : SYSTEM
- PRH  : SYSTEM
*/

#define WARN = MAGENTA
#define ERR "\033[0;31m"

// System Colors |Hardcoded|

#define HOSTCOLOR = RCLR
#define USERCOLOR "\e[0;34m"
#define DIRCOLOR "\e[0;32m"
#define PRH "\e[0;35m"
#define RCLR "\e[0;37m"

void test()
{
    printf("%sERROR%s\nColors still in development, please refer to %sREADME%s\n", ERR, RCLR, ERR, RCLR);
    printf("%sRED\n%sDEFAULT\n%sBLUE\n%sGREEN%s\n", ERR, RCLR, USERCOLOR, DIRCOLOR, RCLR);
}

void resetPrompt()
{
    printf("%s", RCLR);
}

void display_prompt()
{
    char cwd[1024];
    char hostname[1024];
    char *username = getenv("USER");

    gethostname(hostname, sizeof(hostname));
    getcwd(cwd, sizeof(cwd));

    char transformed[1024] = ".(main)";
    char expected_userdir[1024];
    snprintf(expected_userdir, sizeof(expected_userdir), "/home/%s", username);

    if (strncmp(cwd, expected_userdir, strlen(expected_userdir)) == 0)
    {
        // Start with ".(main).User.${username}"
        snprintf(transformed, sizeof(transformed), ".(main).User.%s", username);

        // Get the remaining path after "/home/${username}"
        const char *subpath = cwd + strlen(expected_userdir);

        // Process each part of the remaining path
        while (*subpath)
        {
            if (*subpath == '/')
            {
                strcat(transformed, ".");
            }
            else
            {
                strncat(transformed, subpath, 1);
            }
            subpath++;
        }
    }
    else
    {
        // General transformation for paths outside "/home/${username}"
        char *token = strtok(cwd + 1, "/");
        while (token != NULL)
        {
            strcat(transformed, ".");
            strcat(transformed, token);
            token = strtok(NULL, "/");
        }
    }

    printf(PROMPT, hostname, USERCOLOR, username, RCLR, DIRCOLOR, transformed, RCLR);
}

/* Assume color definitions and other necessary includes are here
int callapi(const char *action, const char *function, const char *state) {
    // Determine the color based on the state
    const char *color;
    if (strcmp(state, "RCLR") == 0) {
        color = NORMAL;  // Use your color macro to set the desired color
    } else if (strcmp(state, "WARN") == 0) {
        color = WARN;
    } else if (strcmp(state, "ERR") == 0) {
        color = ERR;
    } else {
        return -1;  // Invalid state
    }

    // Handle different actions
    if (strcmp(action, "execute") == 0) {
        // Execute a command
        int result = system(function);
        if (result != 0) {
            printf("%sCommand failed: %s%s\n", ERR, function, NORMAL);  // Print error in red
            return result;
        }
        printf("%sCommand executed successfully: %s%s\n", color, function, NORMAL);
        return 0;
    } else if (strcmp(action, "set_prompt") == 0) {
        // Set a new prompt (additional logic needed)
        // E.g., update the prompt variable or function
        return 0;  // Or whatever makes sense
    }

    return -1;  // Unknown action
}
*/

int main()
{
    resetPrompt();
    char command[1024];
    char hostname[1024];

    while (1)
    {
        display_prompt();

        // Get the command from user input
        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            perror("fgets failed");
            exit(1);
        }

        // Removing the newline character
        command[strcspn(command, "\n")] = 0;

        // Check for exit
        if (strcmp(command, "exit") == 0)
        {
            break;
        }
        else if (strcmp(command, "dev") == 0)
        {
            printf("%s\n", dev);
            printf("%s\n", version);
            continue;
        }
        else if (strcmp(command, "testClr") == 0)
        {
            test();
            continue;
        }
        else if (strcmp(command, "OSINFO") == 0)
        {
            gethostname(hostname, sizeof(hostname));
            printf("%sHost%s: %s%s%s\n", PRH, RCLR, ERR, hostname, RCLR);
            printf("%sTerminal%s: %sJSH%s\n", PRH, RCLR, ERR, RCLR);
            //            printf("%sOS%s: %sJ-OS 0.0.4b%s\n\n", PRH, RCLR, ERR, RCLR);
            // Call the external binary `osinfo`
            int result = system("osinfo");

            if (result == -1)
            {
                perror("Failed to execute osinfo");
            }
            continue;
        }
        else if (strcmp(command, "help") == 0)
        {
            printf(HLPMSG, ERR, RCLR, ERR, RCLR, ERR, RCLR);
            continue;
        }
        else if (strcmp(command, "hlp") == 0)
        {
            printf(HLPMSG, ERR, RCLR, ERR, RCLR);
            continue;
        }
        else if (strncmp(command, "cd ", 3) == 0)
        {
            // Extract the path after "cd "
            char *path = command + 3;

            // Attempt to change the directory
            if (chdir(path) != 0)
            {
                perror("cd failed!");
            }
            else
            {
                continue;
            }
        }

        // Execute command
        system(command); // Simplified command execution; would need error handling for full shell
        resetPrompt();
    }

    return 0;
}
