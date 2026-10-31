/* ==============================================================================================
 * Sad Test FMap
 * ============================================================================================== */

#ifndef SAD_TEST_FMAP_H
#define SAD_TEST_FMAP_H

#include "sadlib.h"

typedef struct test_bucket test_bucket;
struct test_bucket {
    int key;
    const char* value;
};

sad_int test_bucket_cmp(const test_bucket* a, const test_bucket* b)
{
    return a->key - b->key;
}

sad_int test_bucket_hash(const test_bucket* bucket)
{
    return bucket->key;
}

static const test_bucket buckets[32] = {
    { 47, "forty seven" },
    { 108, "one hundred eight" },
    { 256, "two hundred fifty six" },
    { 39, "thirty nine" },
    { 182, "one hundred eighty two" },
    { 5, "five" },
    { 311, "three hundred eleven" },
    { 92, "ninety two" },
    { 214, "two hundred fourteen" },
    { 76, "seventy six" },
    { 189, "one hundred eighty nine" },
    { 23, "twenty three" },
    { 134, "one hundred thirty four" },
    { 57, "fifty seven" },
    { 298, "two hundred ninety eight" },
    { 64, "sixty four" },
    { 145, "one hundred forty five" },
    { 8, "eight" },
    { 371, "three hundred seventy one" },
    { 199, "one hundred ninety nine" },
    { 12, "twelve" },
    { 221, "two hundred twenty one" },
    { 83, "eighty three" },
    { 308, "three hundred eight" },
    { 36, "thirty six" },
    { 170, "one hundred seventy" },
    { 49, "forty nine" },
    { 290, "two hundred ninety" },
    { 101, "one hundred one" },
    { 65, "sixty five" },
    { 234, "two hundred thirty four" },
    { 11, "eleven" }
};

static int test_fmap()
{
    sad_int i = 0;
    sad_fmap map;

    sad_fmap_init(&map, sizeof(test_bucket), 32, &test_bucket_cmp);

    for (i = 0; i < 28; i++) {
        sad_fmap_push(&map, buckets + i);
    }

    if (map.len != 28) {
        printf("ERROR: sad_fmap_push() FAILED!\n");
        goto error;
    }

    for (i = 0; i < 28; i++) {
        test_bucket* bucket = (test_bucket*)sad_fmap_get(&map, &buckets[i].key);

        if (!bucket) {
            printf("ERROR: sad_fmap_get() FAILED!\n");
            printf("key: %i\n", buckets[i].key);
            goto error;
        }

        if (bucket->key != buckets[i].key || strcmp(bucket->value, buckets[i].value)) {
            printf("ERROR: sad_fmap_push() or sad_fmap_get() FAILED!\n");
            goto error;
        }
    }

    for (i = 28; i < 32; i++) {
        test_bucket* bucket = (test_bucket*)sad_fmap_get(&map, &buckets[i].key);

        if (bucket) {
            printf("ERROR: sad_fmap_get() FAILED!\n");
            printf("key: %i\n", buckets[i].key);
            goto error;
        }
    }

    for (i = 0; i < 28; i += 2) {
        test_bucket bucket;
        sad_int idx = sad_fmap_pop(&map, &buckets[i].key, &bucket);

        if (idx == SAD_INVALID_INDEX) {
            printf("ERROR: sad_fmap_pop() FAILED!\n");
            printf("key: %i\n", buckets[i].key);
            goto error;
        }

        if (bucket.key != buckets[i].key || strcmp(bucket.value, buckets[i].value)) {
            printf("ERROR: sad_fmap_pop() FAILED!\n");
            goto error;
        }
    }

    for (i = 0; i < 28; i += 2) {
        test_bucket* bucket = (test_bucket*)sad_fmap_get(&map, &buckets[i].key);

        if (bucket) {
            printf("ERROR: sad_fmap_pop() or sad_fmap_get() FAILED!\n");
            printf("key: %i\n", buckets[i].key);
            goto error;
        }
    }

    for (i = 1; i < 28; i += 2) {
        test_bucket* bucket = (test_bucket*)sad_fmap_get(&map, &buckets[i].key);

        if (!bucket) {
            printf("ERROR: sad_fmap_get() FAILED!\n");
            printf("key: %i\n", buckets[i].key);
            goto error;
        }
    }

    sad_fmap_destroy(&map);
    return SAD_OK;

error:
    sad_fmap_destroy(&map);
    return SAD_ERROR;
}

#endif