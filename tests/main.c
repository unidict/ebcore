#include "unity.h"

extern void run_disctype_tests(void);
extern void run_subbook_tests(void);
extern void run_word_tests(void);
extern void run_prefix_tests(void);
extern void run_font_tests(void);

int main(void) {
    UNITY_BEGIN();
    run_disctype_tests();
    run_subbook_tests();
    run_word_tests();
    run_prefix_tests();
    run_font_tests();
    return UNITY_END();
}
