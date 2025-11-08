#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileio.h"

static void usage(const char *prog) {
    fprintf(stderr, "Usage: %s <file>\n", prog);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        usage(argv[0]);
        return 1;
    }

    const char *path = argv[1];
    size_t len = 0;
    char *data = readFile(path, &len);
    if (!data) {
        fprintf(stderr, "mkm: failed to read '%s'\n", path);
        return 1;
    }

    size_t preview = len < 200 ? len : 200;
    fwrite(data, 1, preview, stdout);
    if (len > preview) {
        fputs("\n...\n", stdout);
    }

    free(data);
    return 0;
}