/* ==============================================================================================
 * Sad Test Core
 * ============================================================================================== */

#ifndef SAD_TEST_CORE_H
#define SAD_TEST_CORE_H

#include "sadlib.h"

static int test_clz()
{
    sad_int inputs[4] = { 0x0FF00000, 0, 0x3, 0x10 };
    sad_int expected[4] = { 4, 32, 30, 27 };
    sad_int result = 0;
    sad_int i;

    for (i = 0; i < 4; i++)
    {
        result = sad_clz(inputs[i]);

        if (result != expected[i])
        {
            printf(
                "ERROR: sad_clz() FAILED!\n"
                "INPUT: %d\n"
                "EXPECTED: %d\n"
                "RESULT: %d\n",
                inputs[i],
                expected[i],
                result
            );
            return SAD_ERROR;
        }
    }

    return SAD_OK;
}

static int test_ctz()
{
    sad_int inputs[4] = { 0x0FF00000, 0, 0x3, 0x10 };
    sad_int expected[4] = { 20, 32, 0, 4 };
    sad_int result = 0;
    sad_int i;

    for (i = 0; i < 4; i++)
    {
        result = sad_ctz(inputs[i]);

        if (result != expected[i])
        {
            printf(
                "ERROR: sad_ctz() FAILED!\n"
                "INPUT: %d\n"
                "EXPECTED: %d\n"
                "RESULT: %d\n",
                inputs[i],
                expected[i],
                result
            );
            return SAD_ERROR;
        }
    }

    return SAD_OK;
}

static int test_align()
{
    sad_int inputs[4] = { 3, 21, 15, 32 };
    sad_int expected[4] = { 8, 24, 16, 32 };
    sad_int result = 0;
    sad_int i;

    for (i = 0; i < 4; i++)
    {
        result = (sad_int)sad_align(inputs[i], 8);

        if (result != expected[i])
        {
            printf(
                "ERROR: sad_align() FAILED!\n"
                "INPUT: %d\n"
                "EXPECTED: %d\n"
                "RESULT: %d\n",
                inputs[i],
                expected[i],
                result
            );
            return SAD_ERROR;
        }
    }

    return SAD_OK;
}

static int test_core()
{
    if (test_clz() != SAD_OK)
        return SAD_ERROR;
    if (test_ctz() != SAD_OK)
        return SAD_ERROR;
    if (test_align() != SAD_OK)
        return SAD_ERROR;
    return SAD_OK;
}

#endif