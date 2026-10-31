/* ==============================================================================================
 * Sad Lib Containers
 * ============================================================================================== */

#ifndef SADCONTAINERS_H
#define SADCONTAINERS_H

#include "sadcore.h"

#ifdef __cplusplus
extern "C" {
#endif

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
 * - "off" stores the distance from robin hood hashing and has the special value: 255 = Empty.
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
* helper
* ----------------------------------------------------------------------------------------------- */

SAD_API sad_int sad_get_container_capacity(sad_int len);

/*
* sad_array
* ----------------------------------------------------------------------------------------------- */

SAD_API void sad_array_init(sad_array* arr, sad_int elem_size, sad_int len);
SAD_API void sad_array_destroy(sad_array* arr);
SAD_API void sad_array_reserve(sad_array* arr, sad_int len);
SAD_API void sad_array_trim(sad_array* arr);
SAD_API void* sad_array_get(sad_array* arr, sad_int idx);
SAD_API void sad_array_insert(sad_array* arr, const void* elems, sad_int len, sad_int idx);
SAD_API sad_int sad_array_append(sad_array* arr, const void* elems, sad_int len);
SAD_API void sad_array_erase(sad_array* arr, sad_int idx, sad_int len);
SAD_API sad_int sad_array_push(sad_array* arr, const void* elem);
SAD_API sad_int sad_array_pop(sad_array* arr, void* elem);
SAD_API sad_int sad_array_find(sad_array* arr, const void* elem, sad_cmpfunc cmp);

/*
* sad_string
* ----------------------------------------------------------------------------------------------- */

SAD_API void sad_string_init(sad_string* str, sad_int len);
SAD_API void sad_string_destroy(sad_string* str);
SAD_API void sad_string_reserve(sad_string* str, sad_int len);
SAD_API void sad_string_trim(sad_string* str);
SAD_API char* sad_string_get(sad_string* str, sad_int idx);
SAD_API void sad_string_insert(sad_string* str, const char* seq, sad_int len, sad_int idx);
SAD_API void sad_string_append(sad_string* str, const char* seq, sad_int len);
SAD_API void sad_string_push(sad_string* str, char c);
SAD_API void sad_string_erase(sad_string* str, sad_int idx, sad_int len);
SAD_API sad_int sad_string_cmp(sad_string* a, sad_string* b);
SAD_API sad_int sad_string_hash(sad_string* str);

SAD_API uint8_t* sad_string_to_utf8(sad_string* str, sad_int* out_len);
SAD_API uint16_t* sad_string_to_utf16(sad_string* str, sad_int* out_len);
SAD_API uint32_t* sad_string_to_utf32(sad_string* str, sad_int* out_len);

SAD_API sad_int sad_utf8_encode(uint8_t* dst, uint32_t code_point);
SAD_API sad_int sad_utf8_decode(const uint8_t* seq, uint32_t* out_code_point);
SAD_API uint16_t* sad_utf8_to_utf16(const uint8_t* seq, sad_int* out_len);
SAD_API uint32_t* sad_utf8_to_utf32(const uint8_t* seq, sad_int* out_len);
SAD_API void sad_utf8_to_string(const uint8_t* seq, sad_string* string);

SAD_API sad_int sad_utf16_encode(uint16_t* dst, uint32_t code_point);
SAD_API sad_int sad_utf16_decode(const uint16_t* seq, uint32_t* out_code_point);
SAD_API uint8_t* sad_utf16_to_utf8(const uint16_t* seq, sad_int* out_len);
SAD_API uint32_t* sad_utf16_to_utf32(const uint16_t* seq, sad_int* out_len);
SAD_API void sad_utf16_to_string(const uint16_t* seq, sad_string* string);
SAD_API sad_int sad_utf16_cmp(const uint16_t* a, const uint16_t* b);
SAD_API sad_int sad_utf16_ncmp(const uint16_t* a, const uint16_t* b, sad_int len);

SAD_API uint8_t* sad_utf32_to_utf8(const uint32_t* seq, sad_int* out_len);
SAD_API uint16_t* sad_utf32_to_utf16(const uint32_t* seq, sad_int* out_len);
SAD_API void sad_utf32_to_string(const uint32_t* seq, sad_string* string);
SAD_API sad_int sad_utf32_cmp(const uint32_t* a, const uint32_t* b);
SAD_API sad_int sad_utf32_ncmp(const uint32_t* a, const uint32_t* b, sad_int len);

SAD_API char* sad_encode_base64(const void *data, sad_int size, sad_int* out_len);
SAD_API void* sad_decode_base64(const char* b64, sad_int len, sad_int* out_size);

/*
* sad_fmap
* ----------------------------------------------------------------------------------------------- */

/* TODO
* - add "erase_range" to fmap api, to erase every key between A (inclusive) and B (inclusive)
*/

SAD_API void sad_fmap_init(sad_fmap* map, sad_int bucket_size, sad_int len, sad_cmpfunc cmp);
SAD_API void sad_fmap_destroy(sad_fmap* map);
SAD_API void sad_fmap_reserve(sad_fmap* map, sad_int len);
SAD_API void sad_fmap_trim(sad_fmap* map);
/**
 * @param map target flat map
 * @param bucket bucket to look for
 * @param out_index pointer to int, filled with index of found bucket or where it would be inserted
 * @return pointer to existing bucket or NULL on fail
 */
SAD_API void* sad_fmap_getidx(sad_fmap* map, const void* bucket, sad_int* out_idx);
SAD_API void* sad_fmap_get(sad_fmap* map, const void* bucket);
/**
 * @param map target flat map
 * @param bucket bucket to push into map
 * @return pointer to existing bucket or NULL on success
 */
SAD_API void* sad_fmap_push(sad_fmap* map, const void* bucket);
/**
 * @param map target flat map
 * @param bucket a bucket filled with a key to look for
 * @param out_bucket used to store the found bucket
 * @return index where bucket was removed
 */
SAD_API sad_int sad_fmap_pop(sad_fmap* map, const void* bucket, void* out_bucket);
SAD_API sad_int sad_fmap_erase(sad_fmap* map, const void* bucket);
SAD_API void sad_fmap_pop_idx(sad_fmap* map, sad_int idx, void* out_bucket);
SAD_API void sad_fmap_erase_idx(sad_fmap* map, sad_int idx);

/*
* sad_fdict
* ----------------------------------------------------------------------------------------------- */

SAD_API void sad_fdict_init(sad_fdict* dict, sad_int bucket_size, sad_int len, sad_hashfunc hash, sad_cmpfunc cmp);
SAD_API void sad_fdict_destroy(sad_fdict* dict);
SAD_API void sad_fdict_reserve(sad_fdict* dict, sad_int len);
SAD_API void sad_fdict_trim(sad_fdict* dict);
SAD_API void* sad_fdict_getidx(sad_fdict* dict, const void* bucket, sad_int hash, sad_int* out_idx);
SAD_API void* sad_fdict_get(sad_fdict* dict, const void* bucket);
SAD_API void* sad_fdict_push(sad_fdict* dict, const void* bucket);
SAD_API sad_int sad_fdict_pop(sad_fdict* dict, const void* bucket, void* out_bucket);
SAD_API sad_int sad_fdict_erase(sad_fdict* dict, const void* bucket);
SAD_API void sad_fdict_pop_idx(sad_fdict* dict, sad_int idx, void* out_bucket);
SAD_API void sad_fdict_erase_idx(sad_fdict* dict, sad_int idx);

/*
* sad_sdict
* ----------------------------------------------------------------------------------------------- */

SAD_API void sad_sdict_init(sad_sdict* dict, sad_int bucket_size, sad_int len, sad_hashfunc hash, sad_cmpfunc cmp);
SAD_API void sad_sdict_destroy(sad_sdict* dict);
SAD_API void sad_sdict_reserve(sad_sdict* dict, sad_int len);
SAD_API void sad_sdict_trim(sad_sdict* dict);
SAD_API void* sad_sdict_getidx(sad_sdict* dict, const void* bucket, sad_int hash, sad_int* out_idx);
SAD_API void* sad_sdict_get(sad_sdict* dict, const void* bucket);
SAD_API void* sad_sdict_push(sad_sdict* dict, const void* bucket);
SAD_API sad_int sad_sdict_pop(sad_sdict* dict, const void* bucket, void* out_bucket);
SAD_API sad_int sad_sdict_erase(sad_sdict* dict, const void* bucket);
SAD_API void sad_sdict_pop_idx(sad_sdict* dict, sad_int idx, void* out_bucket);
SAD_API void sad_sdict_erase_idx(sad_sdict* dict, sad_int idx);

#ifdef __cplusplus
}
#endif

#endif /* SADCONTAINERS_H */
