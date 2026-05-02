cmake_minimum_required(VERSION 3.20)
project(ebcore
    VERSION 4.4.3
    LANGUAGES C
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Options
# ---------------------------------------------------------------------------
option(EB_ENABLE_PTHREAD "Enable pthread locking"          OFF)
option(EB_BUILD_TESTS    "Build tests"                      ON)

# ---------------------------------------------------------------------------
# Dependencies
# ---------------------------------------------------------------------------
find_package(ZLIB REQUIRED)

# ---------------------------------------------------------------------------
# Sources
# ---------------------------------------------------------------------------
set(EB_CORE_SOURCES
    src/appendix.c
    src/appsub.c
    src/bcd.c
    src/binary.c
    src/bitmap.c
    src/book.c
    src/booklist.c
    src/copyright.c
    src/cross.c
    src/eb.c
    src/endword.c
    src/error.c
    src/exactword.c
    src/filename.c
    src/font.c
    src/hook.c
    src/jacode.c
    src/keyword.c
    src/lock.c
    src/log.c
    src/match.c
    src/menu.c
    src/multi.c
    src/narwalt.c
    src/narwfont.c
    src/readtext.c
    src/search.c
    src/setword.c
    src/stopcode.c
    src/strcasecmp.c
    src/subbook.c
    src/text.c
    src/widealt.c
    src/widefont.c
    src/word.c
    src/zio.c
)

# ---------------------------------------------------------------------------
# Library
# ---------------------------------------------------------------------------
add_library(eb STATIC ${EB_CORE_SOURCES})

add_library(eb::eb ALIAS eb)

target_include_directories(eb
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/eb>
)

target_compile_definitions(eb
    PUBLIC
)

if(EB_ENABLE_PTHREAD)
    find_package(Threads REQUIRED)
    target_compile_definitions(eb PUBLIC ENABLE_PTHREAD)
    target_link_libraries(eb PUBLIC Threads::Threads)
endif()

target_link_libraries(eb PRIVATE ZLIB::ZLIB)

# ---------------------------------------------------------------------------
# Install
# ---------------------------------------------------------------------------
include(GNUInstallDirs)

install(TARGETS eb
    EXPORT ebcore-targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(EXPORT ebcore-targets
    FILE ebcore-targets.cmake
    NAMESPACE eb::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ebcore
)

set(EB_PUBLIC_HEADERS
    src/appendix.h
    src/binary.h
    src/booklist.h
    src/defs.h
    src/eb.h
    src/error.h
    src/font.h
    src/text.h
    src/zio.h
)

install(FILES ${EB_PUBLIC_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/eb
)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/sysdefs.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/eb
)

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------
if(EB_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
