#include "unity.h"
#include "test_helpers.h"
#include "text.h"

static const char *EXPECTED_HEADINGS[] = {
    "5000 <?> (Kuruu Taoreru)[Chou Tou Jou]",
    "5001 <?> (Yuku)[Chi Ji]",
    "5004 <?> (Motoru Sokonau)[Kou Gyou Kai E]",
    "5007 <?> (Tanoshimu Yorokobu Susumeru)[En On]",
    "500A <?> (Kurushimu)[Sou Su]",
    "500C <?> (Toneri)[Kuwan]",
    "500E <?> (Atsui)[Ten]",
};
#define EXPECTED_PREFIX_COUNT 7

static void test_prefix_returns_expected_count(void)
{
    EB_Book book;
    eb_initialize_library();
    eb_initialize_book(&book);
    eb_bind(&book, get_test_book_path());

    EB_Subbook_Code slist[EB_MAX_SUBBOOKS];
    int scount;
    eb_subbook_list(&book, slist, &scount);
    eb_set_subbook(&book, slist[0]);

    EB_Error_Code ec = eb_search_word(&book, "500");
    TEST_ASSERT_EQUAL_INT(EB_SUCCESS, ec);

    EB_Hit hits[50];
    int hcount, total = 0;
    for (;;) {
        ec = eb_hit_list(&book, 50, hits, &hcount);
        if (ec != EB_SUCCESS || hcount == 0) break;
        total += hcount;
    }
    TEST_ASSERT_EQUAL_INT(EXPECTED_PREFIX_COUNT, total);

    eb_finalize_book(&book);
    eb_finalize_library();
}

static void test_prefix_headings_match(void)
{
    EB_Book book;
    eb_initialize_library();
    eb_initialize_book(&book);
    eb_bind(&book, get_test_book_path());

    EB_Subbook_Code slist[EB_MAX_SUBBOOKS];
    int scount;
    eb_subbook_list(&book, slist, &scount);
    eb_set_subbook(&book, slist[0]);

    eb_search_word(&book, "500");

    EB_Hit hits[50];
    int hcount;
    int idx = 0;

    for (;;) {
        EB_Error_Code ec = eb_hit_list(&book, 50, hits, &hcount);
        if (ec != EB_SUCCESS || hcount == 0) break;
        for (int i = 0; i < hcount; i++) {
            TEST_ASSERT_TRUE(idx < EXPECTED_PREFIX_COUNT);

            char heading[256];
            ssize_t len;
            eb_seek_text(&book, &hits[i].heading);
            eb_read_heading(&book, NULL, NULL, NULL, 255, heading, &len);
            heading[len > 0 ? len : 0] = '\0';
            TEST_ASSERT_EQUAL_STRING(EXPECTED_HEADINGS[idx], heading);
            idx++;
        }
    }
    TEST_ASSERT_EQUAL_INT(EXPECTED_PREFIX_COUNT, idx);

    eb_finalize_book(&book);
    eb_finalize_library();
}

void run_prefix_tests(void)
{
    RUN_TEST(test_prefix_returns_expected_count);
    RUN_TEST(test_prefix_headings_match);
}
