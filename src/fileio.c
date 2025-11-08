#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "fileio.h"

char *readFile(const char *path, size_t *out_len) {
    if (out_len) *out_len = 0;
    if (!path) return NULL;

    FILE *f = fopen(path, "rb");
    if (!f) return NULL;

    if (fseek(f, 0, SEEK_END) != 0) {
        fclose(f);
        return NULL;
    }
    long sz = ftell(f);
    if (sz < 0) {
        fclose(f);
        return NULL;
    }
    if (fseek(f, 0, SEEK_SET) != 0) {
        fclose(f);
        return NULL;
    }

    size_t n = (size_t)sz;
    char *buf = (char *)malloc(n + 1);
    if (!buf) {
        fclose(f);
        return NULL;
    }

    size_t got = fread(buf, 1, n, f);
    fclose(f);

    if (got != n) {
        free(buf);
        return NULL;
    }

    buf[n] = '\0';
    if (out_len) *out_len = n;
    return buf;
}

int writeFile(const char *path, const char *data, size_t len) {
    if (!path || !data) return -1;
    FILE *f = fopen(path, "wb");
    if (!f) return -1;

    size_t left = len;
    const char *p = data;
    while (left > 0) {
        size_t wrote = fwrite(p, 1, left, f);
        if (wrote == 0) {
            int err = ferror(f);
            fclose(f);
            return err ? err : -2;
        }
        p += wrote;
        left -= wrote;
    }
    if (fclose(f) != 0) return -3;
    return 0;
}