#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    int opt;
    char *src_path, *dest_path;
    struct stat st;

    // Parse command-line options
    while ((opt = getopt(argc, argv, "i")) != -1) {
        switch (opt) {
            case 'i':
                // Prompt before overwriting existing file
                break;
            default:
                fprintf(stderr, "Usage: %s [-i] source_file dest_file\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Parse source and destination file paths
    if (optind + 1 >= argc) {
        fprintf(stderr, "Usage: %s [-i] source_file dest_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    src_path = argv[optind];
    dest_path = argv[optind + 1];

    // Check if source file exists and is readable
    if (access(src_path, R_OK) != 0) {
        perror(src_path);
        exit(EXIT_FAILURE);
    }

    // Check if destination file already exists
    if (stat(dest_path, &st) == 0) {
        fprintf(stderr, "%s: File already exists\n", dest_path);
        exit(EXIT_FAILURE);
    }

    // Rename source file to destination file
    if (rename(src_path, dest_path) != 0) {
        perror("rename");
        exit(EXIT_FAILURE);
    }

    return 0;
}

