/* ==============================================================================================
 * Sad Lib Core
 * ============================================================================================== */

#ifndef SADCORE_H
#define SADCORE_H

#include <stddef.h> /* ptrdiff_t, size_t, ... */
#include <stdint.h> /* int8_t, int16_t, ... */
#include <limits.h> /* INT_MAX, UINT_MAX, ... */
#include <assert.h>
#include <stdlib.h> /* malloc, free, ... */
#include <string.h> /* memcpy, memmove, ... */
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
* Compiler detection
* ----------------------------------------------------------------------------------------------- */

#if defined(_MSC_VER)
#define SAD_CC_MSVC 1

#elif defined(__GNUC__) || defined(__MINGW32__) || defined(__MINGW64__)
#define SAD_CC_GCC 1

#elif defined(__clang__)
#define SAD_CC_CLANG 1

#else
#error detection could not find compiler, please implement it

#endif

/*
* Architecture detection
* ----------------------------------------------------------------------------------------------- */

#if defined(_M_AMD64) || defined(_M_X64) || defined(__x86_64__) || defined(__amd64__)
#define SAD_ARCH_X64 1

#elif defined(_M_IX86) || defined(__i386__)
#define SAD_ARCH_X86 1

#elif  defined(_M_ARM64) || defined(__aarch64__) || defined(_M_ARM64EC)
#define SAD_ARCH_ARM64 1

#elif defined(_M_ARM) || defined(__ar_)
#define SAD_ARCH_ARM32 1

#elif defined(__mips__)
#define SAD_ARCH_MIPS 1

#else
#error detection could not find architecture, please implement it

#endif

/*
* OS detection
* ----------------------------------------------------------------------------------------------- */

#if defined(_WIN64) || defined(_WIN32) || defined(WIN32)
#define SAD_OS_WINDOWS 1

#elif defined(__ANDROID__) || defined(ANDROID)
#define SAD_OS_ANDROID 1

#elif defined(__linux__)
#define SAD_OS_LINUX 1

#elif defined(__APPLE__)
#define SAD_OS_MACOS 1

#elif defined(__PSP__)
#define SAD_OS_PSP 1

#else
#error detection could not find system, please implement it

#endif

/*
* Build detection
* ----------------------------------------------------------------------------------------------- */

#if defined(DEBUG) || defined(_DEBUG) || SAD_BUILD_DEBUG || !defined(NDEBUG)
#ifndef SAD_BUILD_DEBUG
#define SAD_BUILD_DEBUG 1
#endif

#else
#ifndef SAD_BUILD_RELEASE
#define SAD_BUILD_RELEASE 1
#endif

#ifndef NDEBUG
#define NDEBUG 1
#endif

#endif

/*
* Arch features
* ----------------------------------------------------------------------------------------------- */

#if defined(SAD_ARCH_X86)
#define SAD_HAS_F64 1
#define CACHE_LINE 64

#endif /* X86 */

#if defined(SAD_ARCH_X64)
#define SAD_HAS_I64 1
#define SAD_HAS_F64 1
#define SAD_CACHE_LINE 64

#endif /* X64 */

#if defined(SAD_ARCH_ARM32)
#define SAD_CACHE_LINE 64

#endif /* ARM32 */

#if defined(SAD_ARCH_ARM64)
#define SAD_HAS_I64 1
#define SAD_CACHE_LINE 64

#endif /* ARM64 */

#if defined(SAD_ARCH_MIPS)
#define SAD_CACHE_LINE 64

#endif /* MIPS */

/*
* Compiler intrinsics
* ----------------------------------------------------------------------------------------------- */

#if defined(SAD_CC_MSVC) /* MSVC */
#define SAD_LIBEXPORT __declspec(dllexport)
#define SAD_LIBIMPORT __declspec(dllimport)
#define SAD_LIBHIDE
#define SAD_FORCEINLINE __forceinline

#define SAD_ALIGNAS(alignment) __declspec(align(alignment))
#define SAD_ALIGNOF(T) __alignof(T)

#endif /* MSVC */

#if defined(SAD_CC_GCC) /* GCC */
#define SAD_LIBEXPORT __attribute__((visibility("default")))
#define SAD_LIBIMPORT
#define SAD_LIBHIDE __attribute__((visibility("hidden")))
#define SAD_FORCEINLINE __attribute__((always_inline))

#define SAD_ALIGNAS(alignment) __attribute__ ((aligned (alignment)))
#define SAD_ALIGNOF(T) __alignof__(T)

#endif /* GCC */

#if defined(SAD_CC_CLANG) /* CLANG */
#define SAD_LIBEXPORT __attribute__((visibility("default")))
#define SAD_LIBIMPORT
#define SAD_LIBHIDE __attribute__((visibility("hidden")))
#define SAD_FORCEINLINE __attribute__((always_inline))

#define SAD_ALIGNAS(alignment) __attribute__ ((aligned (alignment)))
#define SAD_ALIGNOF(T) __alignof__(T)

#endif /* CLANG */

#define SAD_CACHEALIGN SAD_ALIGNAS(CACHE_LINE)

/*
* Sad api
* ----------------------------------------------------------------------------------------------- */

#ifdef SADLIB_BUILD_SHARED
#define SAD_API SAD_LIBEXPORT

#elif defined(SADLIB_USE_SHARED)
#define SAD_API SAD_LIBIMPORT

#else
#define SAD_API

#endif

/*
* Builtin types
* ----------------------------------------------------------------------------------------------- */

/* TODO define stdint.h types if platform/compiler doesnt do it */

/* sad lib integer type used for array- and element sizes */
typedef int sad_int;
typedef unsigned int sad_uint;

/* sad lib integer types used for memory addresses */
typedef ptrdiff_t sad_iptr;
typedef size_t sad_uptr;

/* sad lib default math floating point type */
#if defined(SAD_FLOAT_64)
typedef double sad_float;

#else
typedef float sad_float;

#endif

/*
* Builtin type limits
* ----------------------------------------------------------------------------------------------- */

#define SAD_INT_MIN INT_MIN
#define SAD_INT_MAX INT_MAX
#define SAD_UINT_MAX UINT_MAX

#define SAD_IPTR_MIN INTPTR_MIN
#define SAD_IPTR_MAX INTPTR_MAX
#define SAD_UPTR_MAX SIZE_MAX

#if defined(SAD_FLOAT_64)
#define SAD_FLOAT_EPSILON 1E-9f
#define SAD_FLOAT_MAX	1E+37f
#define SAD_FLOAT_MIN	1E-37f

#else
#define SAD_FLOAT_EPSILON 1E-5f
#define SAD_FLOAT_MAX	1E+37f
#define SAD_FLOAT_MIN	1E-37f

#endif

/*
* Error handling
* ----------------------------------------------------------------------------------------------- */

SAD_API void sad_throw(const char* format, ...);

#define SAD_OK 0
#define SAD_ERROR -1
#define SAD_INVALID_INDEX -1

/*
* Special type functions
* ----------------------------------------------------------------------------------------------- */

SAD_API sad_int sad_clz(sad_uint val);
SAD_API sad_int sad_ctz(sad_uint val);
SAD_API size_t sad_align(size_t val, sad_int alignment);

#ifdef __cplusplus
}
#endif

#endif