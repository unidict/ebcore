# ebcore

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C](https://img.shields.io/badge/C-11-blue.svg)](https://en.wikipedia.org/wiki/C11_(C_standard))
[![CI](https://github.com/unidict/ebcore/actions/workflows/ci.yml/badge.svg)](https://github.com/unidict/ebcore/actions/workflows/ci.yml)

A C library for reading EB/EPWING electronic book dictionaries (JIS X 4081).

## Overview

ebcore is a fork of the [EB Library](http://www.sra.co.jp/people/m-kasahr/eb/) by Motoyuki Kasahara, modernized for cross-platform use:

- Reads EB and EPWING format dictionaries
- Exact word, prefix, end-word, and keyword search
- Text content reading with gaiji (external character) support
- Font bitmap extraction (narrow and wide)
- Cross-platform: macOS, Linux, Windows (MSVC)
- No external dependencies beyond zlib

## Prerequisites

- C11 compiler (GCC, Clang, MSVC)
- zlib >= 1.2
- CMake >= 3.20

## Build

```bash
git clone https://github.com/unidict/ebcore.git
cd ebcore
cmake -B build
cmake --build build
```

### Run Tests

```bash
cmake -B build -DEB_BUILD_TESTS=ON
cmake --build build
ctest --test-dir build --verbose
```

### Install

```bash
cmake --build build --target install
```

## CMake Options

| Option | Default | Description |
|--------|---------|-------------|
| `EB_BUILD_TESTS` | ON | Build test suite |
| `EB_ENABLE_PTHREAD` | OFF | Enable pthread locking |

## Quick Start

```c
#include <eb/eb.h>
#include <eb/error.h>
#include <eb/text.h>

int main(void) {
    EB_Book book;
    EB_Subbook_Code subbooks[EB_MAX_SUBBOOKS];
    int count;

    eb_initialize_library();
    eb_initialize_book(&book);

    if (eb_bind(&book, "/path/to/dictionary") != EB_SUCCESS)
        return 1;

    eb_subbook_list(&book, subbooks, &count);
    eb_set_subbook(&book, subbooks[0]);

    eb_search_exactword(&book, "hello");

    EB_Hit hits[50];
    int hit_count;
    eb_hit_list(&book, 50, hits, &hit_count);

    for (int i = 0; i < hit_count; i++) {
        char heading[256];
        ssize_t len;
        eb_seek_text(&book, &hits[i].heading);
        eb_read_heading(&book, NULL, NULL, NULL, 255, heading, &len);
    }

    eb_finalize_book(&book);
    eb_finalize_library();
    return 0;
}
```

## API Overview

### Book Lifecycle

| Function | Description |
|----------|-------------|
| `eb_initialize_library()` | Initialize the library |
| `eb_finalize_library()` | Finalize the library |
| `eb_initialize_book()` | Initialize a book object |
| `eb_finalize_book()` | Finalize a book object |
| `eb_bind()` | Bind a book to a dictionary path |
| `eb_disc_type()` | Get disc type (EB or EPWING) |

### Subbook

| Function | Description |
|----------|-------------|
| `eb_subbook_list()` | List available subbooks |
| `eb_set_subbook()` | Select a subbook |
| `eb_subbook_title()` | Get subbook title |

### Search

| Function | Description |
|----------|-------------|
| `eb_search_exactword()` | Exact word search |
| `eb_search_word()` | Prefix search |
| `eb_search_endword()` | End-word search |
| `eb_search_keyword()` | Multi-keyword search |
| `eb_hit_list()` | Retrieve search results |

### Text

| Function | Description |
|----------|-------------|
| `eb_seek_text()` | Seek to a text position |
| `eb_read_heading()` | Read entry heading |
| `eb_read_text()` | Read entry body text |

### Font

| Function | Description |
|----------|-------------|
| `eb_font_list()` | List available font sizes |
| `eb_set_font()` | Select font size |
| `eb_font_height()` | Get font height in pixels |
| `eb_narrow_font_character_bitmap()` | Get narrow character bitmap |
| `eb_wide_font_character_bitmap()` | Get wide character bitmap |

## Platform Support

| Platform | Compiler | Status |
|----------|----------|--------|
| macOS | Clang (Xcode) | Tested |
| Linux | GCC / Clang | CI |
| Windows | MSVC | CI |

## License

BSD 3-Clause — based on the original EB Library by Motoyuki Kasahara.

## Acknowledgments

- [EB Library](http://www.sra.co.jp/people/m-kasahr/eb/) by Motoyuki Kasahara
- [Unity Test Framework](http://www.throwtheswitch.org/unity) by ThrowTheSwitch
