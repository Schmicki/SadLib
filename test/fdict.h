/* ==============================================================================================
 * Sad Test FDict
 * ============================================================================================== */

#ifndef SAD_TEST_FDICT_H
#define SAD_TEST_FDICT_H

#include "sadlib.h"

static int test_fdict()
{
    sad_int i = 0;
    sad_fdict map;

    sad_fdict_init(&map, sizeof(test_bucket), 32, &test_bucket_hash, &test_bucket_cmp);

    for (i = 0; i < 28; i++) {
        sad_fdict_push(&map, buckets + i);
    }

    if (map.len != 28) {
        printf("ERROR: sad_fdict_push() FAILED!\n");
        goto error;
    }

    for (i = 0; i < 28; i++) {
        test_bucket* bucket = (test_bucket*)sad_fdict_get(&map, &buckets[i].key);

        if (!bucket) {
            printf("ERROR: sad_fdict_get() FAILED!\n");
            printf("key: %i\n", buckets[i].key);
            goto error;
        }

        if (bucket->key != buckets[i].key || strcmp(bucket->value, buckets[i].value)) {
            printf("ERROR: sad_fdict_push() or sad_fdict_get() FAILED!\n");
            goto error;
        }
    }

    for (i = 28; i < 32; i++) {
        test_bucket* bucket = (test_bucket*)sad_fdict_get(&map, &buckets[i].key);

        if (bucket) {
            printf("ERROR: sad_fdict_get() FAILED!\n");
            printf("key: %i\n", buckets[i].key);
            goto error;
        }
    }

    for (i = 0; i < 28; i += 2) {
        test_bucket bucket;
        sad_int idx = sad_fdict_pop(&map, &buckets[i].key, &bucket);

        if (idx == SAD_INVALID_INDEX) {
            printf("ERROR: sad_fdict_pop() FAILED!\n");
            printf("key: %i\n", buckets[i].key);
            goto error;
        }

        if (bucket.key != buckets[i].key || strcmp(bucket.value, buckets[i].value)) {
            printf("ERROR: sad_fdict_pop() FAILED!\n");
            goto error;
        }
    }

    for (i = 0; i < 28; i += 2) {
        test_bucket* bucket = (test_bucket*)sad_fdict_get(&map, &buckets[i].key);

        if (bucket) {
            printf("ERROR: sad_fdict_pop() or sad_fdict_get() FAILED!\n");
            printf("key: %i\n", buckets[i].key);
            goto error;
        }
    }

    for (i = 1; i < 28; i += 2) {
        test_bucket* bucket = (test_bucket*)sad_fdict_get(&map, &buckets[i].key);

        if (!bucket) {
            printf("ERROR: sad_fdict_get() FAILED!\n");
            printf("key: %i\n", buckets[i].key);
            goto error;
        }
    }

    sad_fdict_destroy(&map);
    return SAD_OK;

error:
    sad_fdict_destroy(&map);
    return SAD_ERROR;
}

#endif