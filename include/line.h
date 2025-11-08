#ifndef MKM_LINE_H
#define MKM_LINE_H

#include <stddef.h>

typedef struct {
    char   **lines;
    size_t   count;
} LineVec;

LineVec lineSplit(const char *data, size_t len);

void lineFree(LineVec *lv);

char *lineJoin(const LineVec *lv, size_t *out_len);

#endif