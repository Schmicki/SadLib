/* ==============================================================================================
 * sad.h - Sad Lib API
 * ============================================================================================== */
#ifndef SADAPI
#define SADAPI 1

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

/* ==============================================================================================
 * Core
 * ============================================================================================== */

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

#else
#define SAD_API SAD_LIBIMPORT

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
typedef double sad_flt;
#else
typedef float sad_flt;
#endif

#ifndef SAD_NO_SHORT_NAMES
typedef sad_int sint;
typedef sad_uint uint;
typedef sad_int flt;
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

/* ==============================================================================================
 * Containers
 * ============================================================================================== */

/*
* Type definitions
* ----------------------------------------------------------------------------------------------- */

typedef sad_int(*sad_hashfunc)(const void*);
typedef sad_int(*sad_cmpfunc)(const void*, const void*);

typedef struct sad_array
{
	void* data;
	sad_int elem_size;
	sad_int len;
	sad_int cap;
} sad_array;

typedef struct sad_string
{
	char* data;
	sad_int len;
	sad_int cap;
} sad_string;

/* TODO sad_text - to handle multi-line text */

/* TODO sad_map - red-black tree based map */

typedef struct sad_fmap
{
	void* buckets;
	sad_int bucket_size;
	sad_int len;
	sad_int cap;
	sad_cmpfunc cmp;
} sad_fmap;

/** #### Flat Dictionary
 * 
 * An array based dictionary using "Robin Hood Hashing". Best used for small to mid-sized
 * dictionaries like below 100000 entries for small data types (1-16 bytes).
 */
typedef struct sad_fdict
{
	void* buckets;
	uint8_t* off;
	sad_int bucket_size;
	sad_int len;
	sad_int cap;
	sad_hashfunc hash;
	sad_cmpfunc cmp;
} sad_fdict;

/** #### Sparse Dictionary
 * 
 * - Robin Hood Hashing
 * - Retains a fifo order
 * - Useful if you need to iterate the contents
 * - off stores the distance from robin hood hashing and has to special values: 255 = Empty.
 */
typedef struct sad_sdict
{
	void* dense;
	sad_int* sparse;
	uint8_t* off;
	uint8_t* info;
	sad_int bucket_size;
	sad_int len;
	sad_int count;
	sad_int cap;
	sad_hashfunc hash;
	sad_cmpfunc cmp;
} sad_sdict;

/*
* sad_array
* ----------------------------------------------------------------------------------------------- */

void sad_arrnew(sad_array* arr, sad_int elem_size, sad_int len);
void sad_arrfree(sad_array* arr);
void sad_arrres(sad_array* arr, sad_int len);
// void sad_arr_trim(sad_array* arr);
void* sad_arrget(sad_array* arr, sad_int idx);
void sad_arrins(sad_array* arr, const void* elems, sad_int len, sad_int idx);
sad_int sad_arrcat(sad_array* arr, const void* elems, sad_int len);
void sad_arrdel(sad_array* arr, sad_int idx, sad_int len);
sad_int sad_arrput(sad_array* arr, const void* elem);
sad_int sad_arrpop(sad_array* arr, void* elem);
sad_int sad_arrfnd(sad_array* arr, const void* elem, sad_int(*cmp)());

/*
* sad_string
* ----------------------------------------------------------------------------------------------- */

void sad_strnew(sad_string* str, sad_int len);
void sad_strfree(sad_string* str);
void sad_strres(sad_string* str, sad_int len);
void sad_strtrim(sad_string* str);
char* sad_strat(sad_string* str, sad_int idx);
void sad_strins(sad_string* str, const char* seq, sad_int len, sad_int idx);
void sad_strcat(sad_string* str, const char* seq, sad_int len);
void sad_strput(sad_string* str, char c);
void sad_strdel(sad_string* str, sad_int idx, sad_int len);
sad_int sad_strcmp(sad_string* a, sad_string* b);
sad_int sad_strhash(sad_string* str);
uint8_t* sad_strtoutf8(sad_string* str, sad_int* out_len);
uint16_t* sad_strtoutf16(sad_string* str, sad_int* out_len);
uint32_t* sad_strtoutf32(sad_string* str, sad_int* out_len);

sad_int sad_utf8enc(uint8_t* dst, uint32_t code_point);
sad_int sad_utf8dec(const uint8_t* seq, uint32_t* out_code_point);
uint16_t* sad_utf8toutf16(const uint8_t* seq, sad_int* out_len);
uint32_t* sad_utf8toutf32(const uint8_t* seq, sad_int* out_len);
void sad_utf8tostr(const uint8_t* seq, sad_string* string);

sad_int sad_utf16enc(uint16_t* dst, uint32_t code_point);
sad_int sad_utf16dec(const uint16_t* seq, uint32_t* out_code_point);
uint8_t* sad_utf16toutf8(const uint16_t* seq, sad_int* out_len);
uint32_t* sad_utf16toutf32(const uint16_t* seq, sad_int* out_len);
void sad_utf16tostr(const uint16_t* seq, sad_string* string);
sad_int sad_utf16cmp(const uint16_t* a, const uint16_t* b);
sad_int sad_utf16ncmp(const uint16_t* a, const uint16_t* b, sad_int len);

uint8_t* sad_utf32toutf8(const uint32_t* seq, sad_int* out_len);
uint16_t* sad_utf32toutf16(const uint32_t* seq, sad_int* out_len);
void sad_utf32tostr(const uint32_t* seq, sad_string* string);
sad_int sad_utf32cmp(const uint32_t* a, const uint32_t* b);
sad_int sad_utf32ncmp(const uint32_t* a, const uint32_t* b, sad_int len);

char* sad_b64enc(const void *data, sad_int size, sad_int* out_len);
void* sad_b64dec(const char* b64, sad_int len, sad_int* out_size);

/*
* sad_fmap
* ----------------------------------------------------------------------------------------------- */

/* TODO
* - add "erase" method to map/dict api
* - add "erase_range" to fmap api, to erase every key between A (inclusive) and B (inclusive)
*/

void sad_fmapnew(sad_fmap* map, sad_int bucket_size, sad_int len, sad_cmpfunc cmp);
void sad_fmapfree(sad_fmap* map);
void sad_fmapres(sad_fmap* map, sad_int len);
void sad_fmaptrim(sad_fmap* map);
/**
 * @param map target flat map
 * @param bucket bucket to look for
 * @param out_index pointer to int, filled with index on success
 * @return pointer to existing bucket or NULL on fail
 */
void* sad_fmapgeti(sad_fmap* map, const void* bucket, sad_int* out_idx);
void* sad_fmapget(sad_fmap* map, const void* bucket);
/**
 * @param map target flat map
 * @param bucket bucket to push into map
 * @return pointer to existing bucket or NULL on success
 */
void* sad_fmapput(sad_fmap* map, const void* bucket);
/* Like fmapput but replaces existing value instead of returning a bucket. */
void sad_fmapset(sad_fmap* map, const void* bucket);
/**
 * @param map target flat map
 * @param bucket a bucket filled with a key to look for
 * @param out_bucket used to store the found bucket
 * @return index where bucket was removed
 */
sad_int sad_fmappop(sad_fmap* map, const void* bucket, void* out_bucket);
sad_int sad_fmapdel(sad_fmap* map, const void* bucket);
void sad_fmappopi(sad_fmap* map, sad_int idx, void* out_bucket);
void sad_fmapdeli(sad_fmap* map, sad_int idx);

/*
* Type definitions
* ----------------------------------------------------------------------------------------------- */

typedef struct sad_vec2
{
	sad_flt x;
	sad_flt y;
} sad_vec2;

typedef struct sad_vec3
{
	sad_flt x;
	sad_flt y;
	sad_flt z;
} sad_vec3;

typedef struct sad_vec4
{
	sad_flt x;
	sad_flt y;
	sad_flt z;
	sad_flt w;
} sad_vec4, sad_quat;

typedef struct sad_mat3
{
	sad_flt m[9];
} sad_mat3;

typedef struct sad_mat4
{
	sad_flt m[16];
} sad_mat4;

typedef struct sad_t3d
{
	sad_vec3 location;
	sad_quat rotation;
	sad_vec3 scale;
} sad_t3d;

/*
* Shapes
* ----------------------------------------------------------------------------------------------- */

typedef struct sad_ray
{
	sad_vec3 origin;
	sad_vec3 direction;
} sad_ray;

typedef struct sad_plane
{
	sad_vec3 normal;
	sad_flt distance;
} sad_plane;

typedef struct sad_line
{
	sad_vec3 start;
	sad_vec3 end;
} sad_line;

typedef struct sad_triangle
{
	sad_vec3 a, b, c;
} sad_triangle;

typedef struct sad_sphere
{
	sad_vec3 origin;
	sad_flt radius;
} sad_sphere;

typedef struct sad_capsule
{
	sad_line line;
	sad_flt radius;
} sad_capsule;

typedef struct sad_aabb
{
	sad_vec3 min, max;
} sad_aabb;

typedef struct sad_obb
{
	sad_vec3 origin;
	sad_vec3 axes[3];
	sad_vec3 extents;
} sad_obb;

/*
* sad_flt
* ----------------------------------------------------------------------------------------------- */

#ifdef SAD_FLOAT_64
#define SAD_SQRT() sqrt
#define SAD_SIN() sin
#define SAD_COS() cos
#define SAD_ABS() abs

#else
#define SAD_SQRT() sqrtf
#define SAD_SIN() sinf
#define SAD_COS() cosf
#define SAD_ABS() absf

#endif

#define SAD_PI (sad_flt)3.141592653589793
#define sad_frtod(r) (r * ((sad_flt)180.0 / SAD_PI))
#define sad_fdtor(d) (d * (SAD_PI / (sad_flt)180.0))

SAD_API sad_flt sad_fabs(sad_flt v);
SAD_API sad_flt sad_fmin(sad_flt a, sad_flt b);
SAD_API sad_flt sad_fmax(sad_flt a, sad_flt b);
SAD_API sad_flt sad_ftrunc(sad_flt v);
SAD_API sad_flt sad_fround(sad_flt v);
SAD_API sad_flt sad_fmod(sad_flt v, sad_flt mod);
SAD_API sad_flt sad_fclamp(sad_flt v, sad_flt min, sad_flt max);
SAD_API sad_int sad_fisclose(sad_flt a, sad_flt b, sad_flt epsilon);
SAD_API sad_flt sad_fsin(sad_flt v);
SAD_API sad_flt sad_fasin(sad_flt v);
SAD_API sad_flt sad_fcos(sad_flt v);
SAD_API sad_flt sad_facos(sad_flt v);
SAD_API sad_flt sad_ftan(sad_flt v);
SAD_API sad_flt sad_fatan(sad_flt v);
SAD_API sad_flt sad_fsqrt(sad_flt v);
SAD_API sad_flt sad_fpow(sad_flt v, sad_flt exp);
SAD_API sad_flt sad_flog2(sad_flt v);
SAD_API sad_int sad_fcmp(sad_flt a, sad_flt b);

/*
* sad_vec2
* ----------------------------------------------------------------------------------------------- */

sad_vec2* sad_v2set(sad_vec2* dst, sad_flt x, sad_flt y);
sad_vec2* sad_v2add(sad_vec2* dst, sad_vec2* b);
sad_vec2* sad_v2sub(sad_vec2* dst, sad_vec2* b);
sad_vec2* sad_v2mul(sad_vec2* dst, sad_flt v);
sad_vec2* sad_v2div(sad_vec2* dst, sad_flt v);
sad_flt sad_v2dot(sad_vec2* a, sad_vec2* b);
sad_flt sad_v2len(sad_vec2* src);
sad_vec2* sad_v2unit(sad_vec2* dst);
sad_vec2* sad_v2lerp(sad_vec2* from_dst, sad_vec2* to, sad_flt alpha);
/* sad_vec2* sad_v2_rotate(sad_vec2* dst, sad_flt degrees); */
/* sad_vec2* sad_v2_transform(sad_vec2* dst, sad_mat3* transform); */
sad_int sad_v2_cmp(sad_vec2* a, sad_vec2* b);

/*
* Closest Point Computation
* ----------------------------------------------------------------------------------------------- */

sad_flt sad_dist_point_plane(sad_plane p, sad_vec3 pt);
sad_flt sad_dist2_point_line(sad_line l, sad_vec3 p);
sad_flt sad_dist2_point_aabb(sad_aabb box, sad_vec3 p);
sad_flt sad_dist2_point_obb(sad_obb box, sad_vec3 p);

sad_vec3 sad_clpt_ray(sad_ray r, sad_vec3 p);
sad_vec3 sad_clpt_plane(sad_plane p, sad_vec3 pt);
sad_vec3 sad_clpt_line(sad_line l, sad_vec3 p);
sad_vec3 sad_clpt_triangle(sad_triangle t, sad_vec3 p);
sad_vec3 sad_clpt_sphere(sad_sphere s, sad_vec3 p);
sad_vec3 sad_clpt_aabb(sad_aabb box, sad_vec3 p);
sad_vec3 sad_clpt_obb(sad_obb box, sad_vec3 p);
sad_flt sad_clpt_line_line(sad_line a, sad_line b, sad_vec3* p1, sad_vec3* p2);

/*
* Collision Detection
* ----------------------------------------------------------------------------------------------- */

sad_int sad_test_point_ray(sad_ray r, sad_vec3 p);
sad_int sad_test_point_line(sad_line l, sad_vec3 p);
sad_int sad_test_point_plane(sad_plane p, sad_vec3 pt);
sad_int sad_test_point_sphere(sad_sphere s, sad_vec3 p);
sad_int sad_test_point_aabb(sad_aabb box, sad_vec3 p);
sad_int sad_test_point_obb(sad_obb box, sad_vec3 p);

sad_int sad_test_sphere_sphere(sad_sphere a, sad_sphere b);
sad_int sad_test_capsule_capsule(sad_capsule a, sad_capsule b);
sad_int sad_test_aabb_aabb(sad_aabb a, sad_aabb b);
sad_int sad_test_obb_obb(sad_obb a, sad_obb b);

sint sad_test_sphere_capsule(sad_sphere s, sad_capsule c);

#ifdef __cplusplus
}
#endif

#endif /* SADAPI */