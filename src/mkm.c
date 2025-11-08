#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fileio.h"
#include "line.h"

static void usage(const char *prog) {
    fprintf(stderr, "Usage:\n");
    fprintf(stderr, "  %s <file>\n", prog);
    fprintf(stderr, "  %s --number <file>\n", prog);
}

static int cmdPreview(const char *path) {
    size_t len = 0;
    char *data = readFile(path, &len);
    if (!data) {
        fprintf(stderr, "error: failed to read '%s'\n", path);
        return 1;
    }
    size_t preview = len < 200 ? len : 200;
    fwrite(data, 1, preview, stdout);
    if (len > preview) fputs("\n...\n", stdout);
    free(data);
    return 0;
}

static int cmdNumber(const char *path) {
    size_t len = 0;
    char *data = readFile(path, &len);
    if (!data) {
        fprintf(stderr, "error: failed to read '%s'\n", path);
        return 1;
    }

    LineVec lv = lineSplit(data, len);
    free(data);

    for (size_t i = 0; i < lv.count; ++i) {
        printf("%zu: %s\n", i + 1, lv.lines[i]);
    }

    lineFree(&lv);
    return 0;
}

int main(int argc, char **argv) {
    if (argc == 2) {
        return cmdPreview(argv[1]);
    } else if (argc == 3 && strcmp(argv[1], "--number") == 0) {
        return cmdNumber(argv[2]);
    } else {
        usage(argv[0]);
        return 1;
    }
}