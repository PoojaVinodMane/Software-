#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <getopt.h>
#include <readline/readline.h>
#include <readline/history.h>

#define BUF_SIZE 4096

int main(int argc, char **argv) {
    int src_fd, dest_fd;
    int c;
    int option_index = 0;
    int preserve_flag = 0;

    // parse command line options
    while ((c = getopt(argc, argv, "p")) != -1) {
        switch (c) {
            case 'p':
                preserve_flag = 1;
                break;
            case '?':
                printf("Unknown option: %c\n", optopt);
                break;
        }
    }

    // read source and destination file paths using readline
    char *src_path = readline("Source file path: ");
    char *dest_path = readline("Destination file path: ");

    // open source file for reading
    src_fd = open(src_path, O_RDONLY);
    if (src_fd < 0) {
        printf("Error: unable to open source file\n");
        exit(1);
    }

    // open destination file for writing
    dest_fd = open(dest_path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (dest_fd < 0) {
        printf("Error: unable to open destination file\n");
        exit(1);
    }

    // copy file data
    char buf[BUF_SIZE];
    ssize_t bytes_read, bytes_written;
    while ((bytes_read = read(src_fd, buf, BUF_SIZE)) > 0) {
        bytes_written = write(dest_fd, buf, bytes_read);
        if (bytes_written < 0) {
            printf("Error: unable to write to destination file\n");
            exit(1);
        }
    }

    // close file descriptors
    close(src_fd);
    close(dest_fd);

    // preserve file attributes if -p option is set
    if (preserve_flag) {
        struct stat src_stats;
        if (stat(src_path, &src_stats) < 0) {
            printf("Error: unable to get source file stats\n");
            exit(1);
        }
        if (chmod(dest_path, src_stats.st_mode) < 0) {
            printf("Error: unable to set file mode\n");
            exit(1);
        }
        if (chown(dest_path, src_stats.st_uid, src_stats.st_gid) < 0) {
            printf("Error: unable to set file owner and group\n");
            exit(1);
        }
    }

    return 0;
}

