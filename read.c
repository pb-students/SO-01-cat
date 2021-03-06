//
// Created by waff on 3/6/21.
//

#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "read.h"

int readFile(char *filename) {
    struct passwd *pw = getpwuid(getuid());
    const char* homeDir = pw->pw_dir;

    // Expand home directory
    if (filename[0] == '~' && filename[1] == '/') {
        unsigned long len = strlen(filename) + strlen(homeDir);

        char* absolutePath = malloc(len * sizeof(char));
        strcpy(absolutePath, homeDir);
        strncat(absolutePath, filename + 1, strlen(filename) - 1);
        absolutePath[len - 1] = '\0';
        filename = absolutePath;
    }

    int fd = open(filename, O_RDONLY);

    if (fd == -1) {
        fprintf(stderr, "Cannot open file %s: ", filename);
        switch (errno) {
            case EACCES: fprintf(stderr, "File is not readable.\n"); break;
            case EINTR: fprintf(stderr, "Interrupted by a signal.\n"); break;
            case EMFILE: fprintf(stderr, "Too many files open.\n"); break;
            case ENOENT: fprintf(stderr, "File does not exist.\n"); break;
        }
        return errno;
    }

    char* buf;
    // NOTE: errno check skipped because the only error that could occur is ESPIPE
    off_t size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);

    buf = malloc(size);

    if (read(fd, buf, size) == -1) {
        fprintf(stderr, "Error reading file %s: ", filename);
        switch (errno) {
            case EINTR: fprintf(stderr, "Interrupted by a signal.\n"); break;
            case EIO: fprintf(stderr, "Hardware error. \n"); break;
            case EINVAL: fprintf(stderr, "Offsets were not properly aligned.\n"); break;
        }
        return errno;
    }

    printf("%s", buf);

    close(fd);
    return 0;
}
