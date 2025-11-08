#include <stdlib.h>
#include <string.h>

#include "line.h"

static void *xmalloc(size_t n) { return malloc(n); }

LineVec lineSplit(const char *data, size_t len) {
    LineVec out = {0};
    if (!data || len == 0) return out;

    size_t lines = 0;
    for(size_t i = 0; i < len; ++i) {
        if (data[i] == '\n'){
            lines++;
        }
    }
    if(len > 0 && data[len - 1] != '\n'){
        lines++;
    }

    if(lines == 0){
        return out;
    }

    char **arr = (char **)xmalloc(lines * sizeof(char *));
    if (!arr){
        return out;
    }

    size_t idx = 0;
    size_t start = 0;

    for (size_t i = 0; i <= len; ++i) {
        int is_end = (i == len) || (data[i] == '\n');
        if (!is_end) continue;

        size_t end = i;
        if (end > start && data[end - 1] == '\r') end--;

        size_t L = end > start ? (end - start) : 0;
        char *line = (char *)xmalloc(L + 1);
        if (!line) {
            for (size_t k = 0; k < idx; ++k) free(arr[k]);
            free(arr);
            return (LineVec){0};
        }
        if (L) memcpy(line, data + start, L);
        line[L] = '\0';

        arr[idx++] = line;
        start = i + 1;
    }

    out.lines = arr;
    out.count = idx;
    return out;
}

void lineFree(LineVec *lv) {
    if (!lv || !lv->lines) return;
    for (size_t i = 0; i < lv->count; ++i) free(lv->lines[i]);
    free(lv->lines);
    lv->lines = NULL;
    lv->count = 0;
}

char *lineJoin(const LineVec *lv, size_t *out_len) {
    if (out_len) *out_len = 0;
    if (!lv || lv->count == 0) {
        char *empty = (char *)xmalloc(1);
        if (!empty) return NULL;
        empty[0] = '\0';
        return empty;
    }

    size_t total = 0;
    for (size_t i = 0; i < lv->count; ++i) {
        total += strlen(lv->lines[i]);
    }

    if (lv->count > 1) total += (lv->count - 1);

    char *buf = (char *)xmalloc(total + 1);
    if (!buf) return NULL;

    size_t pos = 0;
    for (size_t i = 0; i < lv->count; ++i) {
        size_t L = strlen(lv->lines[i]);
        if (L) { memcpy(buf + pos, lv->lines[i], L); pos += L; }
        if (i + 1 < lv->count) { buf[pos++] = '\n'; }
    }
    buf[pos] = '\0';
    if (out_len) *out_len = pos;
    return buf;
}