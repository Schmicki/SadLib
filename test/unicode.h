/* ==============================================================================================
 * Sad Test Unicode
 * ============================================================================================== */

#ifndef SAD_TEST_UNICODE_H
#define SAD_TEST_UNICODE_H

#include "sadlib.h"

static int test_unicode()
{
    const char* base = "Hello World!🤣\n";
    const wchar_t* wbase = L"Hello World!🤣\n";
    const uint32_t ibase[] = {0x0048, 0x0065, 0x006c, 0x006c, 0x006f, 0x0020, 0x0057,
        0x006f, 0x0072, 0x006c, 0x0064, 0x0021, 0x1f923, 0x000a, 0x0000};
    
    char* temp = NULL;
    uint16_t* wtemp = NULL;
    uint32_t* itemp = NULL;
    
    wtemp = sad_utf8_to_utf16(base, NULL);
    if (sad_utf16_cmp(wbase, wtemp)) {
        printf("ERROR: sad_utf8_to_utf16() FAILED!\n");
        goto error;
    }
    free(wtemp);
    wtemp = NULL;
    
    itemp = sad_utf8_to_utf32(base, NULL);
    if (sad_utf32_cmp(ibase, itemp)) {
        int i = 0;
        printf("ERROR: sad_utf8_to_utf32() FAILED!\n");

        for (i = 0; ibase[i] != 0; i++)
            printf("%#x ", ibase[i]);
        printf("\n");

        for (i = 0; itemp[i] != 0; i++)
            printf("%#x ", itemp[i]);
        printf("\n");
        
        goto error;
    }
    free(itemp);
    itemp = NULL;

    temp = sad_utf16_to_utf8(wbase, NULL);
    if (strcmp(base, temp)) {
        printf("ERROR: sad_utf16_to_utf8() FAILED!\n");
        goto error;
    }
    free(temp);
    temp = NULL;
    
    itemp = sad_utf16_to_utf32(wbase, NULL);
    if (sad_utf32_cmp(ibase, itemp)) {
        printf("ERROR: sad_utf16_to_utf32() FAILED!\n");
        goto error;
    }
    free(itemp);
    itemp = NULL;

    temp = sad_utf32_to_utf8(ibase, NULL);
    if (strcmp(base, temp)) {
        printf("ERROR: sad_utf32_to_utf8() FAILED!\n");
        goto error;
    }
    free(temp);
    temp = NULL;
    
    wtemp = sad_utf32_to_utf16(ibase, NULL);
    if (sad_utf16_cmp(wbase, wtemp)) {
        printf("ERROR: sad_utf32_to_utf16() FAILED!\n");
        goto error;
    }
    free(wtemp);
    wtemp = NULL;

    return SAD_OK;

error:
    free(temp);
    free(wtemp);
    free(itemp);
    return SAD_ERROR;
}

#endif