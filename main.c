//
// Created by waff on 3/6/21.
//

#include "read.h"

int main (int argc, char *argv[]) {
    for (int i = 1; i < argc; ++i) {
        int err;
        if ((err = readFile(argv[i]))) {
            return err;
        }
    }
}
