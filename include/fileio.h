#ifndef MKM_FILEIO_H
#define MKM_FILEIO_H

#include <stddef.h>
char *readFile(const char *path, size_t *out_len);

int writeFile(const char *path, const char *data, size_t len);

#endif