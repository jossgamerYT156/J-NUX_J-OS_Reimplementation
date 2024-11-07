#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <include/shared.h>

char* get_os_info(const char* key) {
    int fd = open("/etc/os-release", O_RDONLY);
    if (fd == -1) {
        perror("Failed to open /etc/os-release");
        return NULL;
    }

    FILE *file = fdopen(fd, "r");  // Convert fd to FILE* for easier line-based reading
    if (!file) {
        perror("fdopen failed");
        close(fd);
        return NULL;
    }

    char line[512];
    char *value = NULL;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (strncmp(line, key, strlen(key)) == 0 && line[strlen(key)] == '=') {
            // Extract the value part after '='
            char *start = strchr(line, '=') + 1;
            if (*start == '"') start++;  // Skip opening quote if present

            // Allocate memory for the value
            value = malloc(strlen(start) + 1);
            if (!value) {
                perror("malloc failed");
                fclose(file);
                return NULL;
            }

            // Copy the value into the allocated memory, stopping at closing quote or newline
            strncpy(value, start, strlen(start) + 1);
            value[strcspn(value, "\"\n")] = '\0';  // Remove trailing quote or newline

            fclose(file);  // Close FILE*
            return value;
        }
    }

    fclose(file);  // Close FILE* if key not found
    return NULL;   // Return NULL if key not found
}

int main() {
    char *os_name = get_os_info("NAME");
    char *os_version = get_os_info("VERSION");

    if (os_name) {
        printf("%sOS Name%s: %s\n",ERR, RCLR, os_name);
        free(os_name);  // Free allocated memory
    } else {
        printf("OS Name not found\n");
    }

    if (os_version) {
        printf("%sOS Version%s: %s\n",ERR, RCLR, os_version);
        free(os_version);  // Free allocated memory
    } else {
        printf("OS Version not found\n");
    }

    return 0;
}
