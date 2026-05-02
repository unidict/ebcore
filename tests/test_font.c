#include "unity.h"
#include "test_helpers.h"
#include "font.h"

static void test_font_list_not_empty(void)
{
    EB_Book book;
    eb_initialize_library();
    eb_initialize_book(&book);
    eb_bind(&book, get_test_book_path());

    EB_Subbook_Code slist[EB_MAX_SUBBOOKS];
    int scount;
    eb_subbook_list(&book, slist, &scount);
    eb_set_subbook(&book, slist[0]);

    EB_Font_Code flist[EB_MAX_FONTS];
    int fcount;
    EB_Error_Code ec = eb_font_list(&book, flist, &fcount);
    TEST_ASSERT_EQUAL_INT(EB_SUCCESS, ec);
    TEST_ASSERT_TRUE(fcount > 0);

    eb_finalize_book(&book);
    eb_finalize_library();
}

static void test_font_is_16(void)
{
    EB_Book book;
    eb_initialize_library();
    eb_initialize_book(&book);
    eb_bind(&book, get_test_book_path());

    EB_Subbook_Code slist[EB_MAX_SUBBOOKS];
    int scount;
    eb_subbook_list(&book, slist, &scount);
    eb_set_subbook(&book, slist[0]);

    EB_Font_Code flist[EB_MAX_FONTS];
    int fcount;
    eb_font_list(&book, flist, &fcount);
    TEST_ASSERT_TRUE(fcount >= 1);
    TEST_ASSERT_EQUAL_INT(EB_FONT_16, flist[0]);

    eb_finalize_book(&book);
    eb_finalize_library();
}

static void test_font_height_is_16(void)
{
    EB_Book book;
    eb_initialize_library();
    eb_initialize_book(&book);
    eb_bind(&book, get_test_book_path());

    EB_Subbook_Code slist[EB_MAX_SUBBOOKS];
    int scount;
    eb_subbook_list(&book, slist, &scount);
    eb_set_subbook(&book, slist[0]);

    EB_Font_Code flist[EB_MAX_FONTS];
    int fcount;
    eb_font_list(&book, flist, &fcount);
    eb_set_font(&book, flist[0]);

    int height;
    EB_Error_Code ec = eb_font_height(&book, &height);
    TEST_ASSERT_EQUAL_INT(EB_SUCCESS, ec);
    TEST_ASSERT_EQUAL_INT(16, height);

    eb_finalize_book(&book);
    eb_finalize_library();
}

static void test_has_wide_font_not_narrow(void)
{
    EB_Book book;
    eb_initialize_library();
    eb_initialize_book(&book);
    eb_bind(&book, get_test_book_path());

    EB_Subbook_Code slist[EB_MAX_SUBBOOKS];
    int scount;
    eb_subbook_list(&book, slist, &scount);
    eb_set_subbook(&book, slist[0]);

    EB_Font_Code flist[EB_MAX_FONTS];
    int fcount;
    eb_font_list(&book, flist, &fcount);
    eb_set_font(&book, flist[0]);

    TEST_ASSERT_FALSE(eb_have_narrow_font(&book));
    TEST_ASSERT_TRUE(eb_have_wide_font(&book));

    eb_finalize_book(&book);
    eb_finalize_library();
}

static void test_wide_font_start(void)
{
    EB_Book book;
    eb_initialize_library();
    eb_initialize_book(&book);
    eb_bind(&book, get_test_book_path());

    EB_Subbook_Code slist[EB_MAX_SUBBOOKS];
    int scount;
    eb_subbook_list(&book, slist, &scount);
    eb_set_subbook(&book, slist[0]);

    EB_Font_Code flist[EB_MAX_FONTS];
    int fcount;
    eb_font_list(&book, flist, &fcount);
    eb_set_font(&book, flist[0]);

    int start;
    EB_Error_Code ec = eb_wide_font_start(&book, &start);
    TEST_ASSERT_EQUAL_INT(EB_SUCCESS, ec);
    TEST_ASSERT_EQUAL_INT(0xb021, start);

    eb_finalize_book(&book);
    eb_finalize_library();
}

static void test_wide_font_bitmap_nonzero(void)
{
    EB_Book book;
    eb_initialize_library();
    eb_initialize_book(&book);
    eb_bind(&book, get_test_book_path());

    EB_Subbook_Code slist[EB_MAX_SUBBOOKS];
    int scount;
    eb_subbook_list(&book, slist, &scount);
    eb_set_subbook(&book, slist[0]);

    EB_Font_Code flist[EB_MAX_FONTS];
    int fcount;
    eb_font_list(&book, flist, &fcount);
    eb_set_font(&book, flist[0]);

    int start;
    eb_wide_font_start(&book, &start);

    unsigned char bitmap[EB_SIZE_WIDE_FONT_48];
    EB_Error_Code ec = eb_wide_font_character_bitmap(&book, start, (char *)bitmap);
    TEST_ASSERT_EQUAL_INT(EB_SUCCESS, ec);

    int has_nonzero = 0;
    for (int i = 0; i < (int)sizeof(bitmap); i++) {
        if (bitmap[i] != 0) { has_nonzero = 1; break; }
    }
    TEST_ASSERT_TRUE(has_nonzero);

    eb_finalize_book(&book);
    eb_finalize_library();
}

void run_font_tests(void)
{
    RUN_TEST(test_font_list_not_empty);
    RUN_TEST(test_font_is_16);
    RUN_TEST(test_font_height_is_16);
    RUN_TEST(test_has_wide_font_not_narrow);
    RUN_TEST(test_wide_font_start);
    RUN_TEST(test_wide_font_bitmap_nonzero);
}
