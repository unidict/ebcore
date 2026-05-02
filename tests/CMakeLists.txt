# Unity Test Framework
set(UNITY_SOURCES unity/unity.c)

# Test sources
set(TEST_SOURCES
    test_fixture.c
    main.c
    test_disctype.c
    test_subbook.c
    test_word.c
    test_prefix.c
    test_font.c
    test_util.c
    test_hooks.c
)

add_executable(eb_tests ${UNITY_SOURCES} ${TEST_SOURCES})

target_include_directories(eb_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/unity
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(eb_tests PRIVATE eb)

# iconv (not needed on Windows — test_util.c uses passthrough)
if(NOT WIN32)
    include(CheckLibraryExists)
    check_library_exists(iconv iconv_open "" HAVE_LIBICONV)
    if(HAVE_LIBICONV)
        target_link_libraries(eb_tests PRIVATE iconv)
    endif()
endif()

add_test(NAME EbTestSuite COMMAND eb_tests)
