# SadLib

A general purpose cross-platform C89 library to simplify the creation of tools and video games.

## Design Goals

- **SIMPLE** - A minimalist simple API thats easy to learn, but not always beginner friendly to stay true to its goals.
- **POWERFUL** - Offer many useful and sometimes rare features. Make complex procedures simple for the user.
- **PORTABLE** - Write portable code and implement as many platforms as possible.

## Features

### sadcore.h

- includes c standard library files <stddef.h>, <stdint.h> (if it exists), <limits.h>, <assert.h>, <stdlib.h>, <string.h>, <time.h>
- compiler detection and macro definition `SAD_CC_*`
- cpu architecure detection and macro definition `SAD_ARCH_*`
- os detection and macro definition `SAD_OS_*`
- build detection and macro definition `SAD_BUILD_RELEASE`, `SAD_BUILD_DEBUG`
- cpu feature detection and macro definitions `SAD_HAS_I64`, `SAD_HAS_F64`, `SAD_CACHE_LINE`
- macros for compiler agnostic intrinsics
- typedefs for core data types `sad_int`, `sad_uint`, `sad_iptr`, `sad_uptr`, `sad_float`

### sadcontainers.h

Implementations of various container data structures.

- `sad_array`
- `sad_string` + utf-, 8, 16 and 32 conversion aswell as base64
- `sad_fmap`, flat binary search map
- `sad_fdict`, flat array, open addressing hashmap using robin hood hashing
- `sad_sdict`, sparse hashmap with a dense bucket array using robin hood hashing

### sadmath.h

Implements some int and float math in addition to `vec2`, `vec3`, `vec4`, `mat3` (3x3) and `mat4` (4x4).

## Roadmap

### Generic Function Refactor

Generic functions like `sad_string_hash`, `sad_string_cmp`, `sad_display_res_cmp` should use `const void*` arguments. Casting from a function pointer with typed arguments to `void*` is undefined behaviour and could break on some platforms or compilers.

### sadwindow.h

Opengl compatible window creation and manipulation.

### saddraw.h

Backend agnostic 2d drawing library.

### sadgui.h

User-friendly, declarative, retained mode gui library with support for themes and transitions.

### sadnet.h

Networking with raw tcp and udp and support for http (and ftp?).

### sadjson.h

Fast json parser, dynamic data structures for easy manipulation, good allocation strategy.

### sadxml.h

Xml parser and serialization/marshalling.

### sadhtml.h

Html parser optimized for access for webscraping.

### sadstring.h

Advanced string manipulation, regex, kmp, aho-corasick and more.

### sadfs.h

File and directory manipulation.

### sadalloc.h

Allocators like pool, bump, stack, tlsf.

### sadyaml.h

Fast yaml parser and serialization/marshalling.

### sadntfs.h??

Read raw ntfs filesystem from partition, including deleted structures and restore them.

### sadascii.h

Advanced console output and effects.
