/*
 * test_util.h
 * EUC-JP to UTF-8 conversion helper
 */
#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include <stddef.h>

/* Convert EUC-JP string to UTF-8. Returns malloc'd string, caller must free.
   Returns NULL on failure. */
char *eucjp_to_utf8(const char *src, size_t src_len);

#endif
