#ifndef TEST_HELPERS_H
#define TEST_HELPERS_H

#include <string.h>
#include "eb.h"
#include "error.h"

static const char *get_test_book_path(void)
{
    static char path[1024];
    static int init = 0;
    if (!init) {
        const char *file = __FILE__;
        const char *sep = NULL;
        for (const char *p = file; *p; p++)
            if (*p == '/' || *p == '\\') sep = p;
        if (sep) {
            size_t n = (size_t)(sep - file + 1);
            memcpy(path, file, n);
            strcpy(path + n, "data/gks2/Epwing");
        } else {
            strcpy(path, "data/gks2/Epwing");
        }
        init = 1;
    }
    return path;
}

#endif
