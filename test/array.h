/* ==============================================================================================
 * Sad Test Array
 * ============================================================================================== */

#ifndef SAD_TEST_ARRAY_H
#define SAD_TEST_ARRAY_H

#include "sadlib.h"

static int test_array()
{
    sad_int nums[4] = {1, 2, 3, 4};
    sad_array array;
    sad_int i;

    sad_array_init(&array, sizeof(sad_int), 0);

    if (array.data != NULL ||
        array.len != 0 ||
        array.cap != 0 ||
        array.elem_size != sizeof(sad_int))
    {
        printf("ERROR: sad_array_init() FAILED!\n");
        printf("array.data = %p, expected: %p\n", array.data, NULL);
        printf("array.len = %d, expected: %d\n", array.len, 0);
        printf("array.cap = %d, expected: %d\n", array.cap, 0);
        printf("array.elem_size = %d, expected: %d\n", array.elem_size, (int)sizeof(sad_int));
        return SAD_ERROR;
    }

    sad_array_append(&array, nums, 4);

    if (array.data == NULL ||
        array.len != 4 ||
        array.cap < 4)
    {
        printf("ERROR: sad_array_append() FAILED!\n");
        printf("array.data = %p\n", array.data);
        printf("array.len = %d, expected: %d\n", array.len, 4);
        printf("array.cap = %d\n", array.cap);
        return SAD_ERROR;
    }

    for (i = 0; i < 4; i++)
    {
        sad_int* data = (sad_int*)array.data;
        if (data[i] != (i+1))
        {
            printf("ERROR: sad_array_append() FAILED!\n");
            printf("expected: [ 1 2 3 4 ]\n");
            printf("result: [ ");
            for (i = 0; i < 4; i++)
                printf("%d ", data[i]);
            printf("]\n");
            return SAD_ERROR;
        }
    }

    sad_array_insert(&array, nums, 2, 3);
    
    if (array.data == NULL ||
        array.len != 6 ||
        array.cap < 6)
    {
        printf("ERROR: sad_array_insert() FAILED!\n");
        printf("array.data = %p\n", array.data);
        printf("array.len = %d, expected: %d\n", array.len, 6);
        printf("array.cap = %d\n", array.cap);
        return SAD_ERROR;
    }

    for (i = 0; i < 6; i++)
    {
        const sad_int expected[6] = {1, 2, 3, 1, 2, 4};
        sad_int* data = (sad_int*)array.data;

        if (data[i] != expected[i])
        {
            printf("ERROR: sad_array_insert() FAILED!\n");
            printf("expected: [ 1 2 3 1 2 4 ]\n");
            printf("result: [ ");
            for (i = 0; i < 6; i++)
                printf("%d ", data[i]);
            printf("]\n");
            return SAD_ERROR;
        }
    }

    sad_array_erase(&array, 2, 2);
    
    if (array.data == NULL ||
        array.len != 4 ||
        array.cap < 4)
    {
        printf("ERROR: sad_array_erase() FAILED!\n");
        printf("array.data = %p\n", array.data);
        printf("array.len = %d, expected: %d\n", array.len, 4);
        printf("array.cap = %d\n", array.cap);
        return SAD_ERROR;
    }

    for (i = 0; i < 4; i++)
    {
        const sad_int expected[4] = {1, 2, 2, 4};
        sad_int* data = (sad_int*)array.data;

        if (data[i] != expected[i])
        {
            printf("ERROR: sad_array_erase() FAILED!\n");
            printf("expected: [ 1 2 2 4 ]\n");
            printf("result: [ ");
            for (i = 0; i < 4; i++)
                printf("%d ", data[i]);
            printf("]\n");
            return SAD_ERROR;
        }
    }

    sad_array_destroy(&array);

    return SAD_OK;
}

#endif