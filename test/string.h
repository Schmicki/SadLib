/* ==============================================================================================
 * Sad Test String
 * ============================================================================================== */

#ifndef SAD_TEST_STRING_H
#define SAD_TEST_STRING_H

#include "sadlib.h"

static int test_string()
{
    sad_string str;

    sad_string_init(&str, 0);
    
    if (str.data != NULL ||
        str.len != 0 ||
        str.cap != 0)
    {
        printf("ERROR: sad_string_init() FAILED!\n");
        printf("string.data = %p, expected: %p\n", str.data, NULL);
        printf("string.len = %d, expected: %d\n", str.len, 0);
        printf("string.cap = %d, expected: %d\n", str.cap, 0);
        return SAD_ERROR;
    }

    sad_string_append(&str, "Hello World!\n", 13);
    
    if (str.data == NULL ||
        str.len != 13 ||
        str.cap < 13)
    {
        printf("ERROR: sad_string_append() FAILED!\n");
        printf("string.data = %p\n", str.data);
        printf("string.len = %d, expected: %d\n", str.len, 13);
        printf("string.cap = %d\n", str.cap);
        return SAD_ERROR;
    }

    if (strncmp(str.data, "Hello World!\n", str.len) != 0)
    {
        printf("ERROR: sad_string_append() FAILED!\n");
        printf("result: '%s', expected: '%s'\n", str.data, "Hello World!\\n");
        return SAD_ERROR;
    }

    sad_string_insert(&str, "C ", 2, 6);
    
    if (str.data == NULL ||
        str.len != 15 ||
        str.cap < 15)
    {
        printf("ERROR: sad_string_insert() FAILED!\n");
        printf("string.data = %p\n", str.data);
        printf("string.len = %d, expected: %d\n", str.len, 15);
        printf("string.cap = %d\n", str.cap);
        return SAD_ERROR;
    }

    if (strncmp(str.data, "Hello C World!\n", str.len) != 0)
    {
        printf("ERROR: sad_string_insert() FAILED!\n");
        printf("result: '%s', expected: '%s'\n", str.data, "Hello C World!\\n");
        return SAD_ERROR;
    }

    sad_string_erase(&str, 7, 6);
    
    if (str.data == NULL ||
        str.len != 9 ||
        str.cap < 9)
    {
        printf("ERROR: sad_string_erase() FAILED!\n");
        printf("string.data = %p\n", str.data);
        printf("string.len = %d, expected: %d\n", str.len, 9);
        printf("string.cap = %d\n", str.cap);
        return SAD_ERROR;
    }

    if (strncmp(str.data, "Hello C!\n", str.len) != 0)
    {
        printf("ERROR: sad_string_erase() FAILED!\n");
        printf("result: '%s', expected: '%s'\n", str.data, "Hello C!\\n");
        return SAD_ERROR;
    }

    sad_string_destroy(&str);

    return SAD_OK;
}

#endif