#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

int main(int argc, char *argv[]) {
    int opt, line_num = 0, found = 0;
    char *pattern, *file_path;
    char line[MAX_LINE_LENGTH];
    FILE *fp;

    // Parse command-line options
    while ((opt = getopt(argc, argv, "hi")) != -1) {
        switch (opt) {
            case 'h':
                // Print filename with matching line
                break;
            case 'i':
                // Case-insensitive search
                break;
            default:
                fprintf(stderr, "Usage: %s [-h] [-i] pattern file_path\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Parse search pattern and file path
    if (optind + 1 >= argc) {
        fprintf(stderr, "Usage: %s [-h] [-i] pattern file_path\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    pattern = argv[optind];
    file_path = argv[optind + 1];

    // Open file for reading
    fp = fopen(file_path, "r");
    if (fp == NULL) {
        perror(file_path);
        exit(EXIT_FAILURE);
    }

    // Read each line and search for the pattern
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL) {
        line_num++;

        if (strstr(line, pattern) != NULL) {
            found = 1;

            if (opt == 'h') {
                printf("%s:", file_path);
            }

            printf("%d:%s", line_num, line);
        }
    }

    // Close file
    fclose(fp);

    if (!found) {
        printf("%s: Pattern not found\n", file_path);
    }

    return 0;
}

