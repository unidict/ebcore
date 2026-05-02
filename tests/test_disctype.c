#include "unity.h"
#include "test_helpers.h"

static void test_bind_success(void)
{
    EB_Book book;
    eb_initialize_library();
    eb_initialize_book(&book);
    EB_Error_Code ec = eb_bind(&book, get_test_book_path());
    TEST_ASSERT_EQUAL_INT(EB_SUCCESS, ec);
    eb_finalize_book(&book);
    eb_finalize_library();
}

static void test_bind_invalid_path(void)
{
    EB_Book book;
    eb_initialize_library();
    eb_initialize_book(&book);
    EB_Error_Code ec = eb_bind(&book, "/nonexistent/path");
    TEST_ASSERT_NOT_EQUAL(EB_SUCCESS, ec);
    eb_finalize_book(&book);
    eb_finalize_library();
}

static void test_disc_type_epwing(void)
{
    EB_Book book;
    eb_initialize_library();
    eb_initialize_book(&book);
    eb_bind(&book, get_test_book_path());
    EB_Disc_Code disc;
    EB_Error_Code ec = eb_disc_type(&book, &disc);
    TEST_ASSERT_EQUAL_INT(EB_SUCCESS, ec);
    TEST_ASSERT_EQUAL_INT(EB_DISC_EPWING, disc);
    eb_finalize_book(&book);
    eb_finalize_library();
}

void run_disctype_tests(void)
{
    RUN_TEST(test_bind_success);
    RUN_TEST(test_bind_invalid_path);
    RUN_TEST(test_disc_type_epwing);
}
