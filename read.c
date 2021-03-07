//
// Created by waff on 3/6/21.
//

#include <fcntl.h>
#include <errno.h>
#include <stdio.h>
#include <pwd.h>
#include <unistd.h>
#include <stdlib.h>
#include "read.h"

int readFile(char *filename) {
    char* absolutePath = realpath(filename, NULL);

    if (absolutePath == NULL) {
        fprintf(stderr, "Cannot get absolute path for file %s: ", filename);

        switch (errno) {
            case EACCES: fprintf(stderr, "Path is not readable.\n"); break;
            case EIO: fprintf(stderr, "An I/O error occurred.\n"); break;
            case ELOOP: fprintf(stderr, "Too many symbolic links were encountered in the path.\n"); break;
            case ENAMETOOLONG: fprintf(stderr, "Path is too long.\n"); break;
            case ENOENT: fprintf(stderr, "Path does not exists.\n"); break;
            case ENOMEM: fprintf(stderr, "Out of memory.\n"); break;
            case ENOTDIR: fprintf(stderr, "One of the path's component is not a directory.\n"); break;
        }

        return errno;
    }

    filename = absolutePath;

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
