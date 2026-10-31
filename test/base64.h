/* ==============================================================================================
 * Sad Test Base64
 * ============================================================================================== */

#ifndef SAD_TEST_BASE64_H
#define SAD_TEST_BASE64_H

#include "sadlib.h"

static const char nums[] = "{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15} ";
static const char base64[] = "ezAsIDEsIDIsIDMsIDQsIDUsIDYsIDcsIDgsIDksIDEwLCAxMSwgMTIsIDEzLCAxNCwgMTV9IA==";

static int test_base64()
{
    char* b64 = sad_encode_base64(nums, 55, NULL);
    char* string = sad_decode_base64(base64, SAD_INT_MAX, NULL);

    if (strcmp(base64, b64)) {
        printf("ERROR: sad_encode_base64() FAILED!\n");
        printf("expected: %s\n", base64);
        printf("result: %s\n", b64);
        goto error;
    }

    if (strncmp(nums, string, 55)) {
        printf("ERROR: sad_decode_base64() FAILED!\n");
        printf("expected: %s\n", nums);
        printf("result: %s\n", string);
        goto error;
    }

    free(b64);
    free(string);
    return SAD_OK;

error:
    free(b64);
    free(string);
    return SAD_ERROR;
}

#endif