#include "sadcontainers.h"

/*
* helper
* ----------------------------------------------------------------------------------------------- */

SAD_API sad_int sad_get_container_capacity(sad_int len)
{
	return len < 8 ? 8 : (UINT_MAX >> sad_clz(len - 1)) + 1;
}

/*
* sad_array
* ----------------------------------------------------------------------------------------------- */

SAD_API void sad_array_init(sad_array* arr, sad_int elem_size, sad_int len)
{
    arr->data = NULL;
    arr->elem_size = elem_size;
    arr->len = 0;
    arr->cap = 0;

    sad_array_reserve(arr, len);
}

SAD_API void sad_array_destroy(sad_array* arr)
{
    free(arr->data);
}

SAD_API void sad_array_reserve(sad_array* arr, sad_int len)
{
    if (arr->cap < len)
    {
        void* new_data;

        arr->cap = sad_get_container_capacity(len);
        new_data = realloc(arr->data, (sad_uptr)arr->cap * arr->elem_size);
        arr->data = new_data;
    }
}

SAD_API void sad_array_trim(sad_array* arr)
{
    if ((arr->cap / 4) > arr->len)
    {
        arr->cap = sad_get_container_capacity(arr->len * 2);
        arr->data = realloc(arr->data, (sad_uptr)arr->cap * arr->elem_size);
    }
}

SAD_API void* sad_array_get(sad_array* arr, sad_int idx)
{
    return (void*)((sad_uptr)arr->data + (sad_uptr)idx * arr->elem_size);
}

SAD_API void sad_array_insert(sad_array* arr, const void* elems, sad_int len, sad_int idx)
{
    void* dst, *src;
    size_t size = (sad_uptr)arr->elem_size * len;

#ifdef SAD_BUILD_DEBUG
    if (idx > arr->len)
        sad_throw("[ERROR] sad_array_insert: index out of bounds!");
#endif

    sad_array_reserve(arr, arr->len + len);
    src = (void*)((sad_uptr)arr->data + (sad_uptr)idx * arr->elem_size);
    dst = (void*)((sad_uptr)src + size);
    memmove(dst, src, (sad_uptr)(arr->len - idx) * arr->elem_size);
    memcpy(src, elems, size);
    arr->len += len;
}

SAD_API sad_int sad_array_append(sad_array *arr, const void *elems, sad_int len)
{
    sad_array_insert(arr, elems, len, arr->len);
    return arr->len - len;
}

SAD_API sad_int sad_array_push(sad_array* arr, const void* elem)
{
    sad_array_insert(arr, elem, 1, arr->len);
    return arr->len - 1;
}

SAD_API sad_int sad_array_pop(sad_array *arr, void *elem)
{
    void* src = (void*)((size_t)arr->data + ((size_t)arr->elem_size * --arr->len));

#ifdef SAD_BUILD_DEBUG
    if (arr->len <= 0)
        sad_throw("[ERROR] sad_array_pop: empty!");
#endif

    memcpy(elem, src, arr->elem_size);
    return arr->len;
}

SAD_API void sad_array_erase(sad_array* arr, sad_int idx, sad_int len)
{
    void* dst, *src;
    sad_uptr size;

#ifdef SAD_BUILD_DEBUG
    if ((idx <= 0) || (idx + len > arr->len))
        sad_throw("[ERROR] sad_array_erase: out of bounds!");
#endif
    
    dst = (void*)((sad_uptr)arr->data + (sad_uptr)idx * arr->elem_size);
    src = (void*)((sad_uptr)dst + (size_t)len * arr->elem_size);
    size = (sad_uptr)(arr->len - idx - len) * arr->elem_size;
    arr->len -= len;

    memmove(dst, src, size);
    sad_array_trim(arr);
}

SAD_API sad_int sad_array_find(sad_array* arr, const void* elem, sad_cmpfunc cmp)
{
    sad_int i;
    sad_uptr e, end;

    e = (sad_uptr)arr->data;
    end = e + (sad_uptr)arr->len * arr->elem_size;

    for (i = 0; e < end; e += arr->elem_size, i++)
    {
        if (cmp(elem, (void*)e) == 0)
            return i;
    }

    return SAD_INVALID_INDEX;
}

/*
* sad_string
* ----------------------------------------------------------------------------------------------- */

SAD_API void sad_string_init(sad_string* str, sad_int len)
{
    str->data = NULL;
    str->len = 0;
    str->cap = 0;

    if (len > 0)
    {
        sad_string_reserve(str, 0);
    }
}

SAD_API void sad_string_destroy(sad_string* str)
{
    free(str->data);
}

SAD_API void sad_string_reserve(sad_string* str, sad_int len)
{
    if (str->cap <= len)
    {
        char* new_data;
        str->cap = sad_get_container_capacity(len + 1);
        new_data = (char*)realloc(str->data, (sad_uptr)str->cap);
        str->data = new_data;
    }
}

SAD_API void sad_string_trim(sad_string* str)
{
    if ((str->cap / 4) > str->len)
    {
        // char* new_data;
        str->cap = sad_get_container_capacity(str->len * 2);
        str->data = (char*)realloc(str->data, (sad_uptr)str->cap);
    }
}

SAD_API char* sad_string_get(sad_string* str, sad_int idx)
{
    return str->data + idx;
}

SAD_API void sad_string_insert(sad_string* str, const char* seq, sad_int len, sad_int idx)
{
    sad_string_reserve(str, str->len + len);
    memmove(str->data + idx + len, str->data + idx, str->len - idx);
    memcpy(str->data + idx, seq, len);
    str->len += len;
    str->data[str->len] = 0;
}

SAD_API void sad_string_append(sad_string* str, const char* seq, sad_int len)
{
    sad_string_insert(str, seq, len, str->len);
}

SAD_API void sad_string_push(sad_string *str, char c)
{
    sad_string_reserve(str, str->len + 1);
    str->data[str->len++] = c;
    str->data[str->len] = 0;
}

SAD_API void sad_string_erase(sad_string* str, sad_int idx, sad_int len)
{
    sad_int end;

    if (idx >= str->len)
        return;
    
    end = idx + len;
    
    /* clamp end to string length */
    end -= (end - str->len) * (end > str->len);

    memmove(str->data + idx, str->data + end, str->len - end);
    str->len -= end - idx;
    str->data[str->len] = 0;
    sad_string_trim(str);
}

SAD_API sad_int sad_string_cmp(sad_string* a, sad_string* b)
{
    return strcmp(a->data, b->data);
}

SAD_API sad_int sad_string_hash(sad_string* str)
{
	sad_int multiplier = 31;
	sad_int i, m, hash = 0;

	for (i = 0, m = 1; str->data[i] != 0; m *= multiplier, i++)
		hash += str->data[i] * m;
	return hash;
}

/*
* # UTF-8
* 01111111                             7-bit ASCII characters
* 110xxxxx 10xxxxxx                    2-byte sequence 11-bit characters
* 1110xxxx 10xxxxxx 10xxxxxx           3-byte sequence 16-bit characters
* 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx  4-byte sequence 21-bit characters
*
* ASCII: 0 - 127
*
* Start: 192u - 253u		||	-64 - -3
* Following: 128u - 191u	||	-128 - -65
*
* Start 2: 192u - 223u		||	-64 - -33
* Start 3: 224u - 239u		||	-32 - -17
* Start 4: 240u - 247u		||	-16 - -9
* Start 5: 248u - 251u		||	-8 - -5
* Start 6: 252u - 253u		||	-4 - -3
*
* # UTF-16 - Little Endian
* xxxxxxxx xxxxxxxx                    16-bit Unicode characters
* 110110xx xxxxxxxx                    High
* 110111xx xxxxxxxx                    Low
* 110110xx xxxxxxxx 110110xx xxxxxxxx  4-byte sequence 20-bit charcaters
* 
* The following space is reserved in unicode to identify UTF-16 "Surrogate Pairs".
* - (11011000 00000000 - 11011011 11111111) (0xD800 - 0xDBFF)
* - (11011100 00000000 - 11011111 11111111) (0xDC00 - 0xDFFF)
*/

SAD_API uint8_t* sad_string_to_utf8(sad_string* str, sad_int* out_len)
{
#if CHAR_BIT == 8
    uint8_t* string = malloc(str->len + 1);
    memcpy(string, str->data, str->len + 1);

    if (out_len != NULL)
        *out_len = str->len;
    return string;

#elif CHAR_BIT == 16
    return sad_utf16_to_utf8((uint16_t*)str->data, out_len);

#endif
}

SAD_API uint16_t* sad_string_to_utf16(sad_string* str, sad_int* out_len)
{
#if CHAR_BIT == 8
    return sad_utf8_to_utf16((uint8_t*)str->data, out_len);

#elif CHAR_BIT == 16
    uint16_t* string = malloc(sizeof(uint16_t) * (str->len + 1));
    memcpy(string, str->data, sizeof(uint16_t) * (str->len + 1));

    if (out_len != NULL)
        *out_len = str->len;
    return string;

#endif
}

SAD_API uint32_t* sad_string_to_utf32(sad_string* str, sad_int* out_len)
{
#if CHAR_BIT == 8
    return sad_utf8_to_utf32((uint8_t*)str->data, out_len);

#elif CHAR_BIT == 16
    return sad_utf16_to_utf32((uint16_t*)str->data, out_len);

#endif
}


/*
* utf8
* ----------------------------------------------------------------------------------------------- */

SAD_API sad_int sad_utf8_encode(uint8_t *str, uint32_t code_point)
{
    sad_int len = (code_point > 0x7F) + (code_point > 0x7FF) + (code_point > 0xFFFF);
    sad_int bits = len * 6;
    sad_int i = 0;
    uint8_t header = ~(0xFF >> (len + 1)) * (len != 0);

    str[i++] = header + (code_point >> bits);
    while(bits > 0)
        str[i++] = 0x80 + ((code_point >> (bits -= 6)) & 0x3F);
    return len + 1;
}

SAD_API sad_int sad_utf8_decode(const uint8_t* seq, uint32_t* out_code_point)
{
    uint8_t c = seq[0];
    sad_int len = 1 + (c > 0xBF) + (c > 0xDF) + (c > 0xEF);
    uint32_t cp = c & (0xFF >> len);
    sad_int i = 1;

    while (i < len)
        cp = (cp << 6) + (seq[i++] & 0x3F);
    *out_code_point = cp;
    return len;
}

SAD_API uint16_t* sad_utf8_to_utf16(const uint8_t* seq, sad_int* out_len)
{
    sad_int x, y, len;
    uint16_t* string;
    uint32_t cp;

    if (!seq)
    {
        if (out_len != NULL)
            *out_len = 0;
        return NULL;
    }

    /* Figure out length of input string in output format.
     * How many utf16 segs do I need to hold this utf8 string? */
    for (x = 0, len = 0; seq[x] != 0;)
    {
        sad_int size = 1 + (seq[x] > 0xBF) + (seq[x] > 0xDF) + (seq[x] > 0xEF);
        len += 1 + (size > 3);
        x += size;
    }

    /* allocate new string */
    string = malloc(sizeof(uint16_t) * (len + 1));
    string[len] = 0;

    /* transcode */
    for (x = 0, y = 0; seq[x] != 0;)
    {
        x += sad_utf8_decode(seq + x, &cp);
        y += sad_utf16_encode(string + y, cp);
    }

    return string;
}

SAD_API uint32_t* sad_utf8_to_utf32(const uint8_t* seq, sad_int* out_len)
{
    sad_int x, y, len;
    uint32_t* string;

    if (!seq)
    {
        if (out_len != NULL)
            *out_len = 0;
        return NULL;
    }

    /* Figure out length of input string in output format.
     * How many utf32 code points do I need to hold this utf8 string? */
    for (x = 0, len = 0; seq[x] != 0; len++)
        x += 1 + (seq[x] > 0xBF) + (seq[x] > 0xDF) + (seq[x] > 0xEF);

    /* allocate new string */
    string = malloc(sizeof(uint32_t) * (len + 1));
    string[len] = 0;

    /* transcode */
    for (x = 0, y = 0; seq[x] != 0; y++)
        x += sad_utf8_decode(seq + x, string + y);
    return string;
}

#if CHAR_BIT == 8
SAD_API void sad_utf8_to_string(const uint8_t *seq, sad_string *string)
{
    sad_int len;

    if (!seq)
    {
        string->data = NULL;
        string->len = 0;
        string->cap = 0;
        return;
    }

    len = (sad_int)strlen(seq);
    sad_string_init(string, len);
    sad_string_append(string, (char*)seq, len);
}

#elif CHAR_BIT == 16
SAD_API void sad_utf8_to_string(const uint8_t *seq, sad_string *string)
{
    sad_int x, y, len;
    uint32_t cp;

    if (!seq)
    {
        string->data = NULL;
        string->len = 0;
        string->cap = 0;
        return;
    }

    /* Figure out length of input string in output format.
     * How many utf16 segs do I need to hold this utf8 string? */
    for (x = 0, len = 0; seq[x] != 0;)
    {
        sad_int size = 1 + (seq[x] > 0xBF) + (seq[x] > 0xDF) + (seq[x] > 0xEF);
        len += 1 + (size > 3);
        x += size;
    }

    /* allocate new string */
    sad_string_init(string, len);
    string->data[len] = 0;
    string->len = len;

    /* transcode */
    for (x = 0, y = 0; seq[x] != 0;)
    {
        x += sad_utf8_decode(seq + x, &cp);
        y += sad_utf16_encode(string->data + y, cp);
    }
}

#endif

/*
* utf16
* ----------------------------------------------------------------------------------------------- */

SAD_API sad_int sad_utf16_encode(uint16_t* dst, uint32_t code_point)
{
    if (code_point < 0x10000)
	{
		dst[0] = (uint16_t)code_point;
		return 1;
	}
	else
	{
		code_point -= 0x10000;
		dst[0] = 0xD800 | ((code_point >> 10) & 0x3FF);
		dst[1] = 0xDC00 | (code_point & 0x3FF);
		return 2;
	}
}

SAD_API sad_int sad_utf16_decode(const uint16_t* seq, uint32_t* out_code_point)
{
    sad_int len = 1 + ((seq[0] > 0xD7FF) & (seq[0] < 0xE000));
    uint32_t cp;

    if (len == 1)
    {
        cp = seq[0];
    }
    else
    {
        uint32_t a = seq[0] & 0x3FF;
        uint32_t b = seq[1] & 0x3FF;
        cp = ((a << 10) | b) + 0x10000;
    }
    
    *out_code_point = cp;
    return len;
}

SAD_API uint8_t* sad_utf16_to_utf8(const uint16_t* seq, sad_int* out_len)
{
    sad_int x, y, len;
    uint8_t* string;
    uint32_t cp;

    if (!seq)
    {
        if (out_len != NULL)
            *out_len = 0;
        return NULL;
    }

    /* Figure out length of input string in output format.
     * How many utf8 bytes do I need to hold this utf16 string? */
    for (x = 0, len = 0; seq[x] != 0;)
    {
        sad_int size = 1 + ((seq[x] > 0xD7FF) & (seq[x] < 0xE000));
        len += (size == 2) ? (4) : (1 + (seq[x] > 0x7F) + (seq[x] > 0x7FF));
        x += size;
    }
    
    if (out_len != NULL)
        *out_len = len;

    /* allocate new string */
    string = malloc(sizeof(uint8_t) * (len + 1));
    string[len] = 0;

    /* transcode */
    for (x = 0, y = 0; seq[x] != 0;)
    {
        x += sad_utf16_decode(seq + x, &cp);
        y += sad_utf8_encode(string + y, cp);
    }

    return string;
}

SAD_API uint32_t* sad_utf16_to_utf32(const uint16_t* seq, sad_int* out_len)
{
    sad_int x, y, len;
    uint32_t* string;

    if (!seq)
    {
        if (out_len != NULL)
            *out_len = 0;
        return NULL;
    }

    /* Figure out length of input string in output format.
     * How many utf32 code points do I need to hold this utf16 string? */
    for (x = 0, len = 0; seq[x] != 0; len++)
        x += 1 + ((seq[x] > 0xD7FF) & (seq[x] < 0xE000));
    
    if (out_len != NULL)
        *out_len = len;

    /* allocate new string */
    string = malloc(sizeof(uint32_t) * (len + 1));
    string[len] = 0;

    /* transcode */
    for (x = 0, y = 0; seq[x] != 0; y++)
        x += sad_utf16_decode(seq + x, string + y);
    return string;
}

#if CHAR_BIT == 8
SAD_API void sad_utf16_to_string(const uint16_t *seq, sad_string *string)
{
    sad_int x, y, len;
    uint32_t cp;

    if (!seq)
    {
        string->data = NULL;
        string->len = 0;
        string->cap = 0;
        return;
    }

    /* Figure out length of input string in output format.
     * How many utf8 bytes do I need to hold this utf16 string? */
    for (x = 0, len = 0; seq[x] != 0;)
    {
        sad_int size = 1 + ((seq[x] > 0xD7FF) & (seq[x] < 0xE000));
        len += (size == 2) ? (4) : (1 + (seq[x] > 0x7F) + (seq[x] > 0x7FF));
        x += size;
    }

    /* allocate new string */
    sad_string_init(string, len);
    string->data[len] = 0;
    string->len = len;

    /* transcode */
    for (x = 0, y = 0; seq[x] != 0;)
    {
        x += sad_utf16_decode(seq + x, &cp);
        y += sad_utf8_encode(string->data + y, cp);
    }
}

#elif CHAR_BIT == 16
SAD_API void sad_utf16_to_string(const uint16_t *seq, sad_string *string)
{
    sad_int len;

    if (!seq)
    {
        string->data = NULL;
        string->len = 0;
        string->cap = 0;
        return;
    }

    len = (sad_int)strlen(seq);
    sad_string_init(string, len);
    sad_string_append(string, (char*)seq, len);
}

#endif

SAD_API sad_int sad_utf16_cmp(const uint16_t *a, const uint16_t *b)
{
    sad_int i = 0;
    for (; a[i] && b[i] && a[i] == b[i]; i++);
    return (sad_int)(a[i] - b[i]);
}

SAD_API sad_int sad_utf16_ncmp(const uint16_t *a, const uint16_t *b, sad_int len)
{
    sad_int i = 0;
    for (; i < len && a[i] && b[i] && a[i] == b[i]; i++);
    return (sad_int)(a[i] - b[i]);
}

/*
* utf32
* ----------------------------------------------------------------------------------------------- */

SAD_API uint8_t* sad_utf32_to_utf8(const uint32_t* seq, sad_int* out_len)
{
    sad_int x, y, len;
    uint8_t* string;

    if (!seq)
    {
        if (out_len != NULL)
            *out_len = 0;
        return NULL;
    }

    /* Figure out length of input string in output format.
     * How many utf8 bytes do I need to hold this utf32 string? */
    for (x = 0, len = 0; seq[x] != 0; x++)
        len += 1 + (seq[x] > 0x7F) + (seq[x] > 0x7FF) + (seq[x] > 0xFFFF);
    
    if (out_len != NULL)
        *out_len = len;

    /* allocate new string */
    string = malloc(sizeof(uint8_t) * (len + 1));
    string[len] = 0;

    /* transcode */
    for (x = 0, y = 0; seq[x] != 0;)
        y += sad_utf8_encode(string + y, seq[x++]);
    return string;
}

SAD_API uint16_t* sad_utf32_to_utf16(const uint32_t* seq, sad_int* out_len)
{
    sad_int x, y, len;
    uint16_t* string;

    if (!seq)
    {
        if (out_len != NULL)
            *out_len = 0;
        return NULL;
    }

    /* Figure out length of input string in output format.
     * How many utf16 segs do I need to hold this utf32 string? */
    for (x = 0, len = 0; seq[x] != 0; x++)
        len += 1 + (seq[x] > 0xFFFF);
    
    if (out_len != NULL)
        *out_len = len;

    /* allocate new string */
    string = malloc(sizeof(uint16_t) * (len + 1));
    string[len] = 0;

    /* transcode */
    for (x = 0, y = 0; seq[x] != 0;)
        y += sad_utf16_encode(string + y, seq[x++]);
    return string;
}

#if CHAR_BIT == 8
SAD_API void sad_utf32_to_string(const uint32_t *seq, sad_string *string)
{
    sad_int x, y, len;

    if (!seq)
    {
        string->data = NULL;
        string->len = 0;
        string->cap = 0;
        return;
    }

    /* Figure out length of input string in output format.
     * How many utf8 bytes do I need to hold this utf32 string? */
    for (x = 0, len = 0; seq[x] != 0; x++)
        len += 1 + (seq[x] > 0x7F) + (seq[x] > 0x7FF) + (seq[x] > 0xFFFF);

    /* allocate new string */
    sad_string_init(string, len);
    string->data[len] = 0;
    string->len = len;

    /* transcode */
    for (x = 0, y = 0; seq[x] != 0;)
        y += sad_utf8_encode((uint8_t*)string->data + y, seq[x++]);
}

#elif CHAR_BIT == 16
SAD_API void sad_utf32_to_string(const uint32_t *seq, sad_string *string)
{
    sad_int x, y, len;

    if (!seq)
    {
        string->data = NULL;
        string->len = 0;
        string->cap = 0;
        return;
    }

    /* Figure out length of input string in output format.
     * How many utf16 segs do I need to hold this utf32 string? */
    for (x = 0, len = 0; seq[x] != 0; x++)
        len += 1 + (seq[x] > 0xFFFF);

    /* allocate new string */
    sad_string_init(string, len);
    string->data[len] = 0;
    string->len = len;

    /* transcode */
    for (x = 0, y = 0; seq[x] != 0;)
        y += sad_utf16_encode((uint16_t*)string->data + y, seq[x++]);
}

#endif

SAD_API sad_int sad_utf32_cmp(const uint32_t *a, const uint32_t *b)
{
    sad_int i = 0;
    for (; a[i] && b[i] && a[i] == b[i]; i++);
    return (sad_int)(a[i] - b[i]);
}

SAD_API sad_int sad_utf32_ncmp(const uint32_t *a, const uint32_t *b, sad_int len)
{
    sad_int i = 0;
    for (; i < len && a[i] && b[i] && a[i] == b[i]; i++);
    return (sad_int)(a[i] - b[i]);
}

SAD_API char* sad_encode_base64(const void *data, sad_int size, sad_int* out_len)
{
    const char* table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const uint8_t* bytes = (uint8_t*)data;
    char* string = NULL;
    
    sad_int val = 0, bits = -6;
    sad_int i = 0, len = 0;

    string = (char*)malloc(size / 3 * 4 + 5); /* up to 4 bytes overhead */

    while (i < size) {
        val = (val << 8) + bytes[i++];
        bits += 8;

        while (bits >= 0) {
            string[len++] = table[(val >> bits) & 0x3F];
            bits -= 6;
        }
    }

    if (bits > -6)
        string[len++] = table[(val << -bits) & 0x3F];

    while (len % 4)
        string[len++] = '=';
    string[len] = 0;

    if (out_len)
        *out_len = len;
    return string;
}

SAD_API void* sad_decode_base64(const char* b64, sad_int len, sad_int* out_size)
{
    //                             [00, 01, 02, 03, 04, 05, 06, 07, 08, \t, \n, 0b, 0c, \r, 0e, 0f, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 1a, 1b, 1c, 1d, 1e, 1f,' ',  !,  ",  #,  $,  %,  &,  ',  (,  ),  *,  +,',',  -,  .,  /,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  :,  ;,  <,  =,  >,  ?,  @, A, B, C, D, E, F, G, H, I, J,  K,  L,  M,  N,  O,  P,  Q,  R,  S,  T,  U,  V,  W,  X,  Y,  Z,  [,  \,  ],  ^,  _,  `,  a,  b,  c,  d,  e,  f,  g,  h,  i,  j,  k,  l,  m,  n,  o,  p,  q,  r,  s,  t,  u,  v,  w,  x,  y,  z,  {,  |,  },  ~, 7f]
    static const char table[128] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, 62, -1, 63, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, 63, -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1};
    uint8_t* data = NULL;

    sad_int val = 0, bits = -8;
    sad_int i = 0, size = 0;

    data = (uint8_t*)malloc(len / 4 * 3 + 3); /* up to 3 bytes overhead */

    while (i < len) {
        char c = table[b64[i++]];
        
        if (c == -1) break;
        val = (val << 6) + c;
        bits += 6;

        if (bits >= 0) {
            data[size++] = (uint8_t)(val >> bits);
            bits -= 8;
        }
    }

    if (out_size)
        *out_size = size;
    return data;
}

/*
* sad_fmap
* ----------------------------------------------------------------------------------------------- */

SAD_API void sad_fmap_init(sad_fmap* map, sad_int bucket_size, sad_int len, sad_cmpfunc cmp)
{
    map->buckets = NULL;
    map->bucket_size = bucket_size;
    map->len = 0;
    map->cap = 0;
    map->cmp = cmp;

    sad_fmap_reserve(map, len);
}

SAD_API void sad_fmap_destroy(sad_fmap* map)
{
    free(map->buckets);
}

SAD_API void sad_fmap_reserve(sad_fmap* map, sad_int len)
{
    if (map->cap < len)
    {
        void* new_data;

        map->cap = sad_get_container_capacity(len);
        new_data = realloc(map->buckets, (sad_uptr)map->cap * map->bucket_size);
        map->buckets = new_data;
    }
}

SAD_API void sad_fmap_trim(sad_fmap* map)
{
    if ((map->cap / 4) > map->len)
    {
        map->cap = sad_get_container_capacity(map->len * 2);
        map->buckets = realloc(map->buckets, (sad_uptr)map->cap * map->bucket_size);
    }
}

SAD_API void* sad_fmap_getidx(sad_fmap *map, const void *bucket, sad_int *out_idx)
{
    sad_int low, mid, high;

    low = 0; high = map->len - 1;

    while (low <= high)
    {
        void* _bucket;
        sad_int result;

        mid = low + (high - low) / 2;
        _bucket = (void*)((sad_uptr)map->buckets + (sad_uptr)map->bucket_size * mid);
        result = map->cmp(bucket, _bucket);

        low = result > 0 ? mid + 1 : low;
        high = result < 0 ? mid - 1 : high;

        if (result == 0)
        {
            *out_idx = mid;
            return _bucket;
        }
    }

    *out_idx = low;
    return NULL;
}

SAD_API void* sad_fmap_get(sad_fmap* map, const void* bucket)
{
    sad_int idx;
    return sad_fmap_getidx(map, bucket, &idx);
}

SAD_API void* sad_fmap_push(sad_fmap* map, const void* bucket)
{
    sad_int idx;
    void* _bucket = sad_fmap_getidx(map, bucket, &idx);

    if (_bucket != NULL)
        return _bucket;
    
    sad_array_insert((sad_array*)map, bucket, 1, idx);
    return NULL;
}

SAD_API sad_int sad_fmap_pop(sad_fmap* map, const void* bucket, void* out_bucket)
{
    sad_int idx;
    void* _bucket = sad_fmap_getidx(map, bucket, &idx);

    if (_bucket != NULL)
    {
        memcpy(out_bucket, _bucket, map->bucket_size);
        sad_array_erase((sad_array*)map, idx, 1);
        return idx;
    }

    return SAD_INVALID_INDEX;
}

SAD_API sad_int sad_fmap_erase(sad_fmap *map, const void *bucket)
{
    sad_int idx;
    void* _bucket = sad_fmap_getidx(map, bucket, &idx);

    if (_bucket != NULL)
    {
        sad_array_erase((sad_array*)map, idx, 1);
        return idx;
    }

    return SAD_INVALID_INDEX;
}

SAD_API void sad_fmap_pop_idx(sad_fmap *map, sad_int idx, void* out_bucket)
{
    void* bucket = (void*)((size_t)map->buckets + (size_t)idx * map->bucket_size);

#ifdef SAD_BUILD_DEBUG
    if (idx < 0 || idx >= map->len)
        sad_throw("[ERROR] sad_fmap_pop_idx: index out of bounds!");
#endif

    memcpy(out_bucket, bucket, map->bucket_size);
    sad_array_erase((sad_array*)map, idx, 1);
}

SAD_API void sad_fmap_erase_idx(sad_fmap *map, sad_int idx)
{
#ifdef SAD_BUILD_DEBUG
    if (idx < 0 || idx >= map->len)
        sad_throw("[ERROR] sad_fmap_erase_idx: index out of bounds!");
#endif

    sad_array_erase((sad_array*)map, idx, 1);
}

/*
* sad_fdict
* ----------------------------------------------------------------------------------------------- */

SAD_API void sad_fdict_init(sad_fdict* dict, sad_int bucket_size, sad_int len, sad_hashfunc hash, sad_cmpfunc cmp)
{
    sad_int cap = sad_get_container_capacity(len);
    size_t size = (size_t)(cap + 2) * bucket_size;

    dict->cap = cap;
    dict->buckets = malloc(size + cap);
    dict->off = (void*)((size_t)dict->buckets + size);
    dict->bucket_size = bucket_size;
    dict->len = 0;
    dict->hash = hash;
    dict->cmp = cmp;

	memset(dict->off, -1, dict->cap);
}

SAD_API void sad_fdict_destroy(sad_fdict* dict)
{
    free(dict->buckets);
}

SAD_API void sad_fdict_reserve(sad_fdict* dict, sad_int len)
{
    if (len > (dict->cap - (dict->cap / 4)))
    {
        sad_int i;
        sad_fdict new_dict;
        size_t bucket = (size_t)dict->buckets;

        sad_fdict_init(&new_dict, dict->bucket_size, dict->len * 2, dict->hash, dict->cmp);

        for (i = 0; i < dict->cap; i++, bucket += (size_t)dict->bucket_size)
        {
            if (dict->off[i] != UINT8_MAX)
                sad_fdict_push(&new_dict, (void*)bucket);
        }

        sad_fdict_destroy(dict);
        *dict = new_dict;
    }
}

SAD_API void sad_fdict_trim(sad_fdict* dict)
{
    if (dict->len < (dict->cap / 4))
    {
        sad_int i;
        sad_fdict new_dict;
        size_t bucket = (size_t)dict->buckets;

        sad_fdict_init(&new_dict, dict->bucket_size, dict->len * 2, dict->hash, dict->cmp);

        for (i = 0; i < dict->cap; i++, bucket += (size_t)dict->bucket_size)
        {
            if (dict->off[i] != UINT8_MAX)
                sad_fdict_push(&new_dict, (void*)bucket);
        }

        sad_fdict_destroy(dict);
        *dict = new_dict;
    }
}

SAD_API void* sad_fdict_getidx(sad_fdict *dict, const void *bucket, sad_int hash, sad_int *out_idx)
{
    sad_int mask = dict->cap - 1;
    sad_int idx = hash & mask;
    sad_int distance = 0;

    for (;; idx = (idx + 1) & mask, distance++)
    {
        void* _bucket = (void*)((size_t)dict->buckets + ((size_t)idx * dict->bucket_size));
        sad_int _distance = dict->off[idx];

        if ((_distance == UINT8_MAX) | (distance > _distance))
        {
            *out_idx = idx;
            return NULL;
        }
        else if ((distance == _distance) && (dict->cmp(bucket, _bucket) == 0))
        {
            *out_idx = idx;
            return _bucket;
        }
    }
}

SAD_API void* sad_fdict_get(sad_fdict* dict, const void* bucket)
{
    sad_int idx;
    return sad_fdict_getidx(dict, bucket, dict->hash(bucket), &idx);
}

SAD_API void* sad_fdict_push(sad_fdict* dict, const void* bucket)
{
    sad_int idx, hash_idx, mask, distance, old_cap;
    sad_int hash = dict->hash(bucket);
    void* _bucket = sad_fdict_getidx(dict, bucket, hash, &idx);

    if (_bucket != NULL)
        return _bucket;
    
    old_cap = dict->cap;
    sad_fdict_reserve(dict, dict->len + 1);

    if (dict->cap != old_cap)
        _bucket = sad_fdict_getidx(dict, bucket, hash, &idx);
    
    /* shortcut because sad_get_container_size() returns a power of 2 */
    mask = dict->cap - 1;
    hash_idx = hash & mask;
    distance = ((idx < hash_idx) * dict->cap) + (idx - hash_idx);

    for (;; idx = (idx + 1) & mask, distance++)
    {
        sad_int _distance = dict->off[idx];
        _bucket = (void*)((sad_uptr)dict->buckets + ((sad_uptr)idx * dict->bucket_size));

        if (_distance == UINT8_MAX)
        {
            memcpy(_bucket, bucket, dict->bucket_size);
            dict->off[idx] = distance;
            dict->len++;
            return NULL;
        }
        else if (distance > _distance)
        {
            size_t tmp = (size_t)dict->buckets + ((size_t)dict->cap * dict->bucket_size);
            tmp += dict->bucket_size * ((void*)tmp == bucket);

            memcpy((void*)tmp, _bucket, dict->bucket_size);
            memcpy(_bucket, bucket, dict->bucket_size);

            bucket = (void*)tmp;
            dict->off[idx] = distance;
            distance = _distance;
        }

#ifdef SAD_BUILD_DEBUG
        if (distance == (UINT8_MAX - 1))
            sad_throw("[ERROR] sad_fdict_push: Too many keys on same index!");
#endif
    }
}

SAD_API sad_int sad_fdict_pop(sad_fdict* dict, const void* bucket, void* out_bucket)
{
    sad_int idx;
    
    bucket = sad_fdict_getidx(dict, bucket, dict->hash(bucket), &idx);

    if (bucket == NULL)
        return SAD_INVALID_INDEX;
    
    memcpy(out_bucket, bucket, dict->bucket_size);
    
    sad_fdict_erase_idx(dict, idx);
    return idx;
}

SAD_API sad_int sad_fdict_erase(sad_fdict *dict, const void *bucket)
{
    sad_int idx;

    if (sad_fdict_getidx(dict, bucket, dict->hash(bucket), &idx) == NULL)
        return SAD_INVALID_INDEX;
    
    sad_fdict_erase_idx(dict, idx);
    return idx;
}

SAD_API void sad_fdict_pop_idx(sad_fdict *dict, sad_int idx, void *out_bucket)
{
    void* bucket;
    
    bucket = (void*)((size_t)dict->buckets + ((size_t)idx * dict->bucket_size));
    memcpy(out_bucket, bucket, dict->bucket_size);

    sad_fdict_erase_idx(dict, idx);
}

SAD_API void sad_fdict_erase_idx(sad_fdict *dict, sad_int idx)
{
    sad_int mask, last;
    void* bucket;
    
    mask = dict->cap - 1;
    last = idx;
    bucket = (void*)((size_t)dict->buckets + ((size_t)last * dict->bucket_size));

    while (1)
    {
        sad_int next = (last + 1) & mask;
        uint8_t distance = dict->off[last] + 1;

        while (dict->off[next] == distance)
        {
            last = next;
            next = (next + 1) & mask;
            distance++;
        }
        
        if (last != idx)
        {
            size_t _bucket = (size_t)dict->buckets + ((size_t)last * dict->bucket_size);
            memcpy(bucket, (void*)_bucket, dict->bucket_size);
            bucket = (void*)_bucket;
        }
        
        distance = dict->off[next];

        if ((distance > 0) && (distance < UINT8_MAX))
        {
            dict->off[last] = distance - 1;
            idx = last;
            last = next;
            continue;
        }

        dict->off[last] = UINT8_MAX;
        dict->len--;
        break;
    }

    sad_fdict_trim(dict);
}

/*
* sad_sdict
* ----------------------------------------------------------------------------------------------- */

SAD_API void sad_sdict_init(sad_sdict* dict, sad_int bucket_size, sad_int len, sad_hashfunc hash, sad_cmpfunc cmp)
{
    void* mem;
    const sad_int cap = sad_get_container_capacity(len);
    const size_t dense_size = sad_align((size_t)cap * bucket_size, sizeof(sad_int));
    const size_t sparse_size = sizeof(sad_int) * cap;
    const size_t off_size = cap;
    const size_t info_size = cap;
    
    mem = malloc(dense_size + sparse_size + off_size + info_size);

    dict->dense = mem;
    dict->sparse = (sad_int*)((size_t)mem + dense_size);
    dict->off = (uint8_t*)(dict->sparse + cap);
    dict->info = dict->off + cap;

    dict->bucket_size = bucket_size;
    dict->cap = cap;
    dict->len = 0;
    dict->count = 0;
    dict->hash = hash;
    dict->cmp = cmp;
    
    /* reset robin hood distances */
	memset(dict->off, -1, dict->cap);
    /* reset tombstone info */
	memset(dict->info, 0, dict->cap);
}

SAD_API void sad_sdict_destroy(sad_sdict* dict)
{
    free(dict->dense);
}

SAD_API void sad_sdict_reserve(sad_sdict* dict, sad_int len)
{
    if (len > (dict->cap - (dict->cap / 4)))
    {
        sad_int i;
        sad_sdict new_dict;
        size_t bucket = (size_t)dict->dense;

        sad_sdict_init(&new_dict, dict->bucket_size, dict->len * 2, dict->hash, dict->cmp);

        for (i = 0; i < dict->len; i++, bucket += (size_t)dict->bucket_size)
        {
            if (dict->info[i] == 0)
                sad_sdict_push(&new_dict, (void*)bucket);
        }

        sad_sdict_destroy(dict);
        *dict = new_dict;
    }
}

SAD_API void sad_sdict_trim(sad_sdict* dict)
{
    if (dict->count < (dict->cap / 4))
    {
        sad_int i;
        sad_sdict new_dict;
        size_t bucket = (size_t)dict->dense;

        sad_sdict_init(&new_dict, dict->bucket_size, dict->count * 2, dict->hash, dict->cmp);

        for (i = 0; i < dict->len; i++, bucket += (size_t)dict->bucket_size)
        {
            if (dict->info[i] == 0)
                sad_sdict_push(&new_dict, (void*)bucket);
        }

        sad_sdict_destroy(dict);
        *dict = new_dict;
    }
}

SAD_API void *sad_sdict_getidx(sad_sdict *dict, const void *bucket, sad_int hash, sad_int *out_idx)
{
    sad_int mask = dict->cap - 1;
    sad_int idx = hash & mask;
    sad_int distance = 0;

    for (;; idx = (idx + 1) & mask, distance++)
    {
        void* _bucket;
        sad_int _distance = dict->off[idx];

        if ((_distance == UINT8_MAX) | (distance > _distance))
        {
            *out_idx = idx;
            return NULL;
        }

        _bucket = (void*)((size_t)dict->dense + ((size_t)dict->sparse[idx] * dict->bucket_size));
        
        if ((distance == _distance) && (dict->cmp(bucket, _bucket) == 0))
        {
            *out_idx = idx;
            return _bucket;
        }
    }
}

SAD_API void* sad_sdict_get(sad_sdict* dict, const void* bucket)
{
	sad_int idx;
	return sad_sdict_getidx(dict, bucket, dict->hash(bucket), &idx);
}

SAD_API void* sad_sdict_push(sad_sdict* dict, const void* bucket)
{
    sad_int idx, hash_idx, dense_idx, mask, distance, old_cap;
    sad_int hash = dict->hash(bucket);
    void* _bucket = sad_sdict_getidx(dict, bucket, hash, &idx);

    if (_bucket != NULL)
        return _bucket;
    
    old_cap = dict->cap;
    sad_sdict_reserve(dict, dict->len + 1);

    if (dict->cap != old_cap)
        _bucket = sad_sdict_getidx(dict, bucket, hash, &idx);
    
	/* Add bucket to dense */
	_bucket = (void*)((size_t)dict->dense + ((size_t)dict->len * dict->bucket_size));
	memcpy(_bucket, bucket, dict->bucket_size);
	dense_idx = dict->len;
    dict->count++;
    dict->len++;
    
    /* shortcut because sad_get_container_size() returns a power of 2 */
    mask = dict->cap - 1;
    hash_idx = hash & mask;
    distance = ((idx < hash_idx) * dict->cap) + (idx - hash_idx);

    for (;; idx = (idx + 1) & mask, distance++)
    {
        sad_int _distance = dict->off[idx];

        if (_distance == UINT8_MAX)
        {
            dict->sparse[idx] = dense_idx;
            dict->off[idx] = distance;
            return NULL;
        }
        else if (distance > _distance)
        {
			/* Replace index */
            sad_int tmp = dict->sparse[idx];
			dict->sparse[idx] = dense_idx;
            dict->off[idx] = distance;

            dense_idx = tmp;
            distance = _distance;
        }

#ifdef SAD_BUILD_DEBUG
        if (distance == (UINT8_MAX - 1))
            sad_throw("[ERROR] sad_fdict_push: Too many keys on same index!");
#endif
    }
}

SAD_API sad_int sad_sdict_pop(sad_sdict* dict, const void* bucket, void* out_bucket)
{
    sad_int idx;
    
    bucket = sad_sdict_getidx(dict, bucket, dict->hash(bucket), &idx);

    if (bucket == NULL)
        return SAD_INVALID_INDEX;
    
    memcpy(out_bucket, bucket, dict->bucket_size);
    sad_sdict_erase_idx(dict, idx);
    return idx;
}

SAD_API sad_int sad_sdict_erase(sad_sdict *dict, const void *bucket)
{
    sad_int idx;

    if (sad_sdict_getidx(dict, bucket, dict->hash(bucket), &idx) == NULL)
        return SAD_INVALID_INDEX;
    
    sad_sdict_erase_idx(dict, idx);
    return idx;
}

SAD_API void sad_sdict_pop_idx(sad_sdict *dict, sad_int idx, void *out_bucket)
{
    void* bucket;
    
    bucket = (void*)((size_t)dict->dense + ((size_t)dict->sparse[idx] * dict->bucket_size));
    memcpy(out_bucket, bucket, dict->bucket_size);

    sad_sdict_erase_idx(dict, idx);
}

SAD_API void sad_sdict_erase_idx(sad_sdict *dict, sad_int idx)
{
    sad_int mask, last;

    dict->info[dict->sparse[idx]] = 1;
    mask = dict->cap - 1;
    last = idx;

    while (1)
    {
        sad_int next = (last + 1) & mask;
        uint8_t distance = dict->off[last] + 1;

        while (dict->off[next] == distance)
        {
            last = next;
            next = (next + 1) & mask;
            distance++;
        }
        
        dict->sparse[idx] = dict->sparse[last];
        distance = dict->off[next];

        if ((distance > 0) && (distance < UINT8_MAX))
        {
            dict->off[last] = distance - 1;
            idx = last;
            last = next;
            continue;
        }

        dict->off[last] = UINT8_MAX;
        dict->count--;
        break;
    }

    sad_sdict_trim(dict);
}
