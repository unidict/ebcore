#include <stdio.h>
#include <string.h>

#include "eb.h"
#include "error.h"
#include "text.h"
#include "font.h"

static EB_Error_Code
render_bitmap_inline(EB_Book *book, int char_number, int is_wide)
{
    int width = is_wide ? 16 : 8;
    int bytes_per_row = width / 8;
    unsigned char bitmap[32];
    char buf[64];
    int row, col, height;
    EB_Error_Code error_code;

    if (is_wide) {
        error_code = eb_wide_font_character_bitmap(book, char_number, (char *)bitmap);
        if (error_code != EB_SUCCESS) {
            error_code = eb_narrow_font_character_bitmap(book, char_number, (char *)bitmap);
            if (error_code == EB_SUCCESS) { width = 8; bytes_per_row = 1; }
        }
    } else {
        error_code = eb_narrow_font_character_bitmap(book, char_number, (char *)bitmap);
        if (error_code != EB_SUCCESS) {
            error_code = eb_wide_font_character_bitmap(book, char_number, (char *)bitmap);
            if (error_code == EB_SUCCESS) { width = 16; bytes_per_row = 2; }
        }
    }

    if (error_code != EB_SUCCESS) {
        snprintf(buf, sizeof(buf), "[G:%04X]", char_number & 0xffff);
        return eb_write_text_string(book, buf);
    }

    eb_write_text_string(book, "\n");

    {
        int fh;
        if (eb_font_height(book, &fh) != EB_SUCCESS)
            fh = 16;
        height = fh;
    }

    for (row = 0; row < height; row++) {
        int pos = 0;
        for (col = 0; col < width; col++) {
            int byte_idx = row * bytes_per_row + (col / 8);
            int bit = (bitmap[byte_idx] >> (7 - (col % 8))) & 1;
            buf[pos++] = bit ? '#' : '.';
        }
        buf[pos] = '\n';
        buf[pos + 1] = '\0';
        eb_write_text_string(book, buf);
    }

    eb_write_text_string(book, "\n");
    return EB_SUCCESS;
}

static EB_Error_Code
hook_narrow_gaiji(EB_Book *book, EB_Appendix *appendix,
    void *container, EB_Hook_Code hook_code, int argc,
    const unsigned int *argv)
{
    if (argc >= 1)
        return render_bitmap_inline(book, argv[0], 0);
    return eb_write_text_string(book, "[G:?]");
}

static EB_Error_Code
hook_wide_gaiji(EB_Book *book, EB_Appendix *appendix,
    void *container, EB_Hook_Code hook_code, int argc,
    const unsigned int *argv)
{
    if (argc >= 1)
        return render_bitmap_inline(book, argv[0], 1);
    return eb_write_text_string(book, "[G:?]");
}

void setup_gaiji_hooks(EB_Hookset *hookset)
{
    EB_Hook hook;

    hook.code = EB_HOOK_NARROW_FONT;
    hook.function = hook_narrow_gaiji;
    eb_set_hook(hookset, &hook);

    hook.code = EB_HOOK_WIDE_FONT;
    hook.function = hook_wide_gaiji;
    eb_set_hook(hookset, &hook);
}
