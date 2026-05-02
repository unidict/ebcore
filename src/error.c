/*
 * Copyright (c) 1997-2006  Motoyuki Kasahara
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "build-pre.h"
#include "eb.h"
#include "error.h"
#include "build-post.h"

/*
 * Error code strings.
 */
static const char * const error_strings[] = {
    /* 0 -- 4 */
    "EB_SUCCESS",
    "EB_ERR_MEMORY_EXHAUSTED",
    "EB_ERR_EMPTY_FILE_NAME",
    "EB_ERR_TOO_LONG_FILE_NAME",
    "EB_ERR_BAD_FILE_NAME",

    /* 5 -- 9 */
    "EB_ERR_BAD_DIR_NAME",
    "EB_ERR_TOO_LONG_WORD",
    "EB_ERR_BAD_WORD",
    "EB_ERR_EMPTY_WORD",
    "EB_ERR_FAIL_GETCWD",

    /* 10 -- 14 */
    "EB_ERR_FAIL_OPEN_CAT",
    "EB_ERR_FAIL_OPEN_CATAPP",
    "EB_ERR_FAIL_OPEN_TEXT",
    "EB_ERR_FAIL_OPEN_FONT",
    "EB_ERR_FAIL_OPEN_APP",

    /* 15 -- 19 */
    "EB_ERR_FAIL_OPEN_BINARY",
    "EB_ERR_FAIL_READ_CAT",
    "EB_ERR_FAIL_READ_CATAPP",
    "EB_ERR_FAIL_READ_TEXT",
    "EB_ERR_FAIL_READ_FONT",

    /* 20 -- 24 */
    "EB_ERR_FAIL_READ_APP",
    "EB_ERR_FAIL_READ_BINARY",
    "EB_ERR_FAIL_SEEK_CAT",
    "EB_ERR_FAIL_SEEK_CATAPP",
    "EB_ERR_FAIL_SEEK_TEXT",

    /* 25 -- 29 */
    "EB_ERR_FAIL_SEEK_FONT",
    "EB_ERR_FAIL_SEEK_APP",
    "EB_ERR_FAIL_SEEK_BINARY",
    "EB_ERR_UNEXP_CAT",
    "EB_ERR_UNEXP_CATAPP",

    /* 30 -- 34 */
    "EB_ERR_UNEXP_TEXT",
    "EB_ERR_UNEXP_FONT",
    "EB_ERR_UNEXP_APP",
    "EB_ERR_UNEXP_BINARY",
    "EB_ERR_UNBOUND_BOOK",

    /* 35 -- 39 */
    "EB_ERR_UNBOUND_APP",
    "EB_ERR_NO_SUB",
    "EB_ERR_NO_APPSUB",
    "EB_ERR_NO_FONT",
    "EB_ERR_NO_TEXT",

    /* 40 -- 44 */
    "EB_ERR_NO_STOPCODE",
    "EB_ERR_NO_ALT",
    "EB_ERR_NO_CUR_SUB",
    "EB_ERR_NO_CUR_APPSUB",
    "EB_ERR_NO_CUR_FONT",

    /* 45 -- 49 */
    "EB_ERR_NO_CUR_BINARY",
    "EB_ERR_NO_SUCH_SUB",
    "EB_ERR_NO_SUCH_APPSUB",
    "EB_ERR_NO_SUCH_FONT",
    "EB_ERR_NO_SUCH_CHAR_BMP",

    /* 50 -- 54 */
    "EB_ERR_NO_SUCH_CHAR_TEXT",
    "EB_ERR_NO_SUCH_SEARCH",
    "EB_ERR_NO_SUCH_HOOK",
    "EB_ERR_NO_SUCH_BINARY",
    "EB_ERR_DIFF_CONTENT",

    /* 55 -- 59 */
    "EB_ERR_NO_PREV_SEARCH",
    "EB_ERR_NO_SUCH_MULTI_ID",
    "EB_ERR_NO_SUCH_ENTRY_ID",
    "EB_ERR_TOO_MANY_WORDS",
    "EB_ERR_NO_WORD",

    /* 60 -- 64 */
    "EB_ERR_NO_CANDIDATES",
    "EB_ERR_END_OF_CONTENT",
    "EB_ERR_NO_PREV_SEEK",
    "EB_ERR_EBNET_UNSUPPORTED",
    "EB_ERR_EBNET_FAIL_CONNECT",

    /* 65 -- 69 */
    "EB_ERR_EBNET_SERVER_BUSY",
    "EB_ERR_EBNET_NO_PERMISSION",
    "EB_ERR_UNBOUND_BOOKLIST",
    "EB_ERR_NO_SUCH_BOOK",

    NULL
};

/*
 * Look up the error message corresponding to the error code `error_code'.
 */
const char *
eb_error_string(EB_Error_Code error_code)
{
    const char *string;

    if (0 <= error_code && error_code < EB_NUMBER_OF_ERRORS)
        string = error_strings[error_code];
    else
        string = "EB_ERR_UNKNOWN";

    return string;
}


/*
 * Error messages.
 */
static const char * const error_messages[] = {
    /* 0 -- 4 */
    "no error",
    "memory exhausted",
    "an empty file name",
    "too long file name",
    "bad file name",

    /* 5 -- 9 */
    "bad directory name",
    "too long word",
    "a word contains bad character",
    "an empty word",
    "failed to get the current working directory",

    /* 10 -- 14 */
    "failed to open a catalog file",
    "failed to open an appendix catalog file",
    "failed to open a text file",
    "failed to open a font file",
    "failed to open an appendix file",

    /* 15 -- 19 */
    "failed to open a binary file",
    "failed to read a catalog file",
    "failed to read an appendix catalog file",
    "failed to read a text file",
    "failed to read a font file",

    /* 20 -- 24 */
    "failed to read an appendix file",
    "failed to read a binary file",
    "failed to seek a catalog file",
    "failed to seek an appendix catalog file",
    "failed to seek a text file",

    /* 25 -- 29 */
    "failed to seek a font file",
    "failed to seek an appendix file",
    "failed to seek a binary file",
    "unexpected format in a catalog file",
    "unexpected format in an appendix catalog file",

    /* 30 -- 34 */
    "unexpected format in a text file",
    "unexpected format in a font file",
    "unexpected format in an appendix file",
    "unexpected format in a binary file",
    "book not bound",

    /* 35 -- 39 */
    "appendix not bound",
    "no subbook",
    "no subbook in the appendix",
    "no font",
    "no text file",

    /* 40 -- 44 */
    "no stop-code",
    "no alternation string",
    "no current subbook",
    "no current appendix subbook",
    "no current font",

    /* 45 -- 49 */
    "no current binary",
    "no such subbook",
    "no such appendix subbook",
    "no such font",
    "no such character bitmap",

    /* 50 -- 54 */
    "no such character text",
    "no such search method",
    "no such hook",
    "no such binary",
    "different content type",

    /* 55 -- 59 */
    "no previous search",
    "no such multi search",
    "no such multi search entry",
    "too many words specified",
    "no word specified",

    /* 60 -- 64 */
    "no candidates",
    "end of content",
    "no previous seek",
    "ebnet is not supported",
    "failed to connect to an ebnet server",

    /* 65 -- 69 */
    "ebnet server is busy",
    "no access permission",
    "booklist not bound",
    "no such book",

    NULL
};

/*
 * Look up the error message corresponding to the error code `error_code'.
 */
const char *
eb_error_message(EB_Error_Code error_code)
{
    const char *message;

    if (0 <= error_code && error_code < EB_NUMBER_OF_ERRORS)
        message = error_messages[error_code];
    else
        message = "unknown error";

    return message;
}
