/* config.h - platform detection (static, no CMake generation needed) */

#define VERSION "4.4.3"

/*
 * Platform-specific definitions.
 */
#ifdef _WIN32
#  define DOS_FILE_PATH 1
#  ifdef _MSC_VER
#    define EINTR 0
#  endif
#else
#  define _FILE_OFFSET_BITS 64
#endif
