#include "unity.h"
#include <stdlib.h>
#include "test_helpers.h"
#include "text.h"
#include "font.h"
#include "test_util.h"
#include "test_hooks.h"

/* Expected heading for exactword("5001") in gks2 (with NULL hookset) */
static const char EXPECTED_HEADING_5001[] =
    "5001 <?> (Yuku)[Chi Ji]";

/* Expected text body for "5001" with gaiji rendered as ASCII bitmaps (UTF-8) */
static const char EXPECTED_TEXT_5001[] =
    "5001 \n"
    "....#..#........\n"
    "....#..#........\n"
    "...##..#....####\n"
    "...#..#####.#..#\n"
    "..##.#..#...#..#\n"
    "..#.....#...#..#\n"
    ".##.....#...#..#\n"
    "#.#..########..#\n"
    "..#.....#...#..#\n"
    "..#.....#...#..#\n"
    "..#.....#...#..#\n"
    "..#....###..#..#\n"
    "..#....#.#..#..#\n"
    "..#...##.##.####\n"
    "..#..##...##....\n"
    "..#.##..........\n"
    "\n"
    "K区点:1742, JIS16:314A, SJIS16:8969, "
    "Mo:743, 部首:9[2画]人 \n"
    "................\n"
    "..........#.....\n"
    ".........##.....\n"
    ".........#......\n"
    "........##......\n"
    "........#.......\n"
    ".......##.......\n"
    "......###.......\n"
    ".....##.#.......\n"
    "....##..#.......\n"
    "........#.......\n"
    "........#.......\n"
    "........#.......\n"
    "........#.......\n"
    "........#.......\n"
    "................\n"
    "\n"
    " \n"
    ".......#........\n"
    "......###.......\n"
    "......#.##......\n"
    ".....##..##.....\n"
    "....##....##....\n"
    "...##......##...\n"
    "..##........##..\n"
    "###..........###\n"
    "................\n"
    "................\n"
    "................\n"
    "................\n"
    "................\n"
    "................\n"
    "................\n"
    "................\n"
    "\n"
    ", 総画数:10, 訓:Yuku, 音:Chi Ji\n\n";

static void test_exactword_returns_one_hit(void)
{
    EB_Book book;
    eb_initialize_library();
    eb_initialize_book(&book);
    eb_bind(&book, get_test_book_path());

    EB_Subbook_Code slist[EB_MAX_SUBBOOKS];
    int scount;
    eb_subbook_list(&book, slist, &scount);
    eb_set_subbook(&book, slist[0]);

    TEST_ASSERT_TRUE(eb_have_word_search(&book));

    EB_Error_Code ec = eb_search_exactword(&book, "5001");
    TEST_ASSERT_EQUAL_INT(EB_SUCCESS, ec);

    EB_Hit hits[50];
    int hcount;
    int total = 0;
    for (;;) {
        ec = eb_hit_list(&book, 50, hits, &hcount);
        if (ec != EB_SUCCESS || hcount == 0) break;
        total += hcount;
    }
    TEST_ASSERT_EQUAL_INT(1, total);

    eb_finalize_book(&book);
    eb_finalize_library();
}

static void test_exactword_heading_matches(void)
{
    EB_Book book;
    eb_initialize_library();
    eb_initialize_book(&book);
    eb_bind(&book, get_test_book_path());

    EB_Subbook_Code slist[EB_MAX_SUBBOOKS];
    int scount;
    eb_subbook_list(&book, slist, &scount);
    eb_set_subbook(&book, slist[0]);

    eb_search_exactword(&book, "5001");

    EB_Hit hits[50];
    int hcount;
    eb_hit_list(&book, 50, hits, &hcount);
    TEST_ASSERT_TRUE(hcount >= 1);

    char heading[256];
    ssize_t len;
    eb_seek_text(&book, &hits[0].heading);
    EB_Error_Code ec = eb_read_heading(&book, NULL, NULL, NULL, 255, heading, &len);
    TEST_ASSERT_EQUAL_INT(EB_SUCCESS, ec);
    TEST_ASSERT_TRUE(len > 0);
    heading[len] = '\0';
    TEST_ASSERT_EQUAL_STRING(EXPECTED_HEADING_5001, heading);

    eb_finalize_book(&book);
    eb_finalize_library();
}

static void test_exactword_text_matches(void)
{
    EB_Book book;
    EB_Hookset hookset;
    eb_initialize_library();
    eb_initialize_book(&book);
    eb_initialize_hookset(&hookset);
    setup_gaiji_hooks(&hookset);
    eb_bind(&book, get_test_book_path());

    EB_Subbook_Code slist[EB_MAX_SUBBOOKS];
    int scount;
    eb_subbook_list(&book, slist, &scount);
    eb_set_subbook(&book, slist[0]);

    EB_Font_Code flist[EB_MAX_FONTS];
    int fcount;
    eb_font_list(&book, flist, &fcount);
    if (fcount > 0)
        eb_set_font(&book, flist[0]);

    eb_search_exactword(&book, "5001");

    EB_Hit hits[50];
    int hcount;
    eb_hit_list(&book, 50, hits, &hcount);
    TEST_ASSERT_TRUE(hcount >= 1);

    char text[8192];
    ssize_t len;
    eb_seek_text(&book, &hits[0].text);
    EB_Error_Code ec = eb_read_text(&book, NULL, &hookset, NULL, 8191, text, &len);
    TEST_ASSERT_EQUAL_INT(EB_SUCCESS, ec);
    TEST_ASSERT_TRUE(len > 0);

    char *utf8 = eucjp_to_utf8(text, len);
    TEST_ASSERT_EQUAL_STRING(EXPECTED_TEXT_5001, utf8);
    free(utf8);

    eb_finalize_hookset(&hookset);
    eb_finalize_book(&book);
    eb_finalize_library();
}

void run_word_tests(void)
{
    RUN_TEST(test_exactword_returns_one_hit);
    RUN_TEST(test_exactword_heading_matches);
    RUN_TEST(test_exactword_text_matches);
}
