/*
 * test_util.c
 * EUC-JP to UTF-8 conversion (iconv on Unix, Win32 API on Windows)
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test_util.h"

#ifndef _WIN32
#include <iconv.h>
#else
#include <windows.h>
#endif

char *eucjp_to_utf8(const char *src, size_t src_len)
{
    if (src_len == 0) {
        char *empty = malloc(1);
        empty[0] = '\0';
        return empty;
    }

#ifdef _WIN32
    /* EUC-JP → wide char (UTF-16) → UTF-8 */
    int wlen = MultiByteToWideChar(20932, 0, src, (int)src_len, NULL, 0);
    if (wlen == 0) {
        char *dup = malloc(src_len + 1);
        memcpy(dup, src, src_len);
        dup[src_len] = '\0';
        return dup;
    }
    wchar_t *wbuf = malloc((wlen + 1) * sizeof(wchar_t));
    MultiByteToWideChar(20932, 0, src, (int)src_len, wbuf, wlen);
    wbuf[wlen] = L'\0';

    int u8len = WideCharToMultiByte(CP_UTF8, 0, wbuf, wlen, NULL, 0, NULL, NULL);
    if (u8len == 0) {
        free(wbuf);
        char *dup = malloc(src_len + 1);
        memcpy(dup, src, src_len);
        dup[src_len] = '\0';
        return dup;
    }
    char *out = malloc(u8len + 1);
    WideCharToMultiByte(CP_UTF8, 0, wbuf, wlen, out, u8len, NULL, NULL);
    out[u8len] = '\0';
    free(wbuf);
    return out;
#else
    iconv_t cd = iconv_open("UTF-8", "EUC-JP");
    if (cd == (iconv_t)-1) {
        char *dup = malloc(src_len + 1);
        memcpy(dup, src, src_len);
        dup[src_len] = '\0';
        return dup;
    }

    size_t out_len = src_len * 3 + 1;
    char *out = malloc(out_len);
    char *outp = out;
    size_t out_left = out_len - 1;
    char *inp = (char *)src;
    size_t in_left = src_len;

    if (iconv(cd, &inp, &in_left, &outp, &out_left) == (size_t)-1) {
        iconv_close(cd);
        free(out);
        char *dup = malloc(src_len + 1);
        memcpy(dup, src, src_len);
        dup[src_len] = '\0';
        return dup;
    }

    *outp = '\0';
    iconv_close(cd);
    return out;
#endif
}
