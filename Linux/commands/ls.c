#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int opt;
    char *dir_path;
    DIR *dir;
    struct dirent *entry;
    struct stat stat_buf;
    char date[80];

    // Parse command-line options
    while ((opt = getopt(argc, argv, "al")) != -1) {
        switch (opt) {
            case 'a':
                // Include hidden files in the list
                break;
            case 'l':
                // Long format: show detailed information for each file
                break;
            default:
                fprintf(stderr, "Usage: %s [-a] [-l] [directory]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Parse directory path
    if (optind >= argc) {
        dir_path = ".";
    } else {
        dir_path = argv[optind];
    }

    // Open directory for reading
    dir = opendir(dir_path);
    if (dir == NULL) {
        perror(dir_path);
        exit(EXIT_FAILURE);
    }

    // Read directory entries and apply options
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.' && opt != 'a') {
            continue; // Skip hidden files
        }

        if (opt == 'l') {
            if (stat(entry->d_name, &stat_buf) == -1) {
                perror(entry->d_name);
                continue;
            }

            strftime(date, 80, "%b %d %Y %H:%M", localtime(&stat_buf.st_mtime));
            printf("%-10s %3ld %s %s\n", (S_ISDIR(stat_buf.st_mode)) ? "directory" : "file", stat_buf.st_size, date, entry->d_name);
        } else {
            printf("%s\n", entry->d_name);
        }
    }

    // Close directory
    closedir(dir);

    return 0;
}

