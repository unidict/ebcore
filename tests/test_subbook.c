#include "unity.h"
#include "test_helpers.h"

/* gks2 subbook title in EUC-JP: fullwidth "gks2" */
static const char EXPECTED_TITLE_EUCJP[] =
    "\xa3\xe7\xa3\xeb\xa3\xf3\xa3\xb2";

static void test_subbook_count(void)
{
    EB_Book book;
    eb_initialize_library();
    eb_initialize_book(&book);
    eb_bind(&book, get_test_book_path());

    EB_Subbook_Code slist[EB_MAX_SUBBOOKS];
    int scount;
    EB_Error_Code ec = eb_subbook_list(&book, slist, &scount);
    TEST_ASSERT_EQUAL_INT(EB_SUCCESS, ec);
    TEST_ASSERT_EQUAL_INT(1, scount);

    eb_finalize_book(&book);
    eb_finalize_library();
}

static void test_subbook_title(void)
{
    EB_Book book;
    eb_initialize_library();
    eb_initialize_book(&book);
    eb_bind(&book, get_test_book_path());

    EB_Subbook_Code slist[EB_MAX_SUBBOOKS];
    int scount;
    eb_subbook_list(&book, slist, &scount);

    char title[EB_MAX_TITLE_LENGTH + 1];
    EB_Error_Code ec = eb_subbook_title2(&book, slist[0], title);
    TEST_ASSERT_EQUAL_INT(EB_SUCCESS, ec);
    TEST_ASSERT_EQUAL_STRING(EXPECTED_TITLE_EUCJP, title);

    eb_finalize_book(&book);
    eb_finalize_library();
}

static void test_subbook_directory(void)
{
    EB_Book book;
    eb_initialize_library();
    eb_initialize_book(&book);
    eb_bind(&book, get_test_book_path());

    EB_Subbook_Code slist[EB_MAX_SUBBOOKS];
    int scount;
    eb_subbook_list(&book, slist, &scount);

    char dir[EB_MAX_DIRECTORY_NAME_LENGTH + 1];
    EB_Error_Code ec = eb_subbook_directory2(&book, slist[0], dir);
    TEST_ASSERT_EQUAL_INT(EB_SUCCESS, ec);
    TEST_ASSERT_EQUAL_STRING("gks2", dir);

    eb_finalize_book(&book);
    eb_finalize_library();
}

void run_subbook_tests(void)
{
    RUN_TEST(test_subbook_count);
    RUN_TEST(test_subbook_title);
    RUN_TEST(test_subbook_directory);
}
