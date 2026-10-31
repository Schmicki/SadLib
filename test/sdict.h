/* ==============================================================================================
 * Sad Test SDict
 * ============================================================================================== */

#ifndef SAD_TEST_SDICT_H
#define SAD_TEST_SDICT_H

#include "sadlib.h"

static int test_sdict()
{
    sad_int i = 0;
    sad_sdict map;

    sad_sdict_init(&map, sizeof(test_bucket), 32, &test_bucket_hash, &test_bucket_cmp);

    for (i = 0; i < 28; i++) {
        sad_sdict_push(&map, buckets + i);
    }

    if (map.len != 28) {
        printf("ERROR: sad_sdict_push() FAILED!\n");
        goto error;
    }

    for (i = 0; i < 28; i++) {
        test_bucket* bucket = (test_bucket*)sad_sdict_get(&map, &buckets[i].key);

        if (!bucket) {
            printf("ERROR: sad_sdict_get() FAILED!\n");
            printf("key: %i\n", buckets[i].key);
            goto error;
        }

        if (bucket->key != buckets[i].key || strcmp(bucket->value, buckets[i].value)) {
            printf("ERROR: sad_sdict_push() or sad_sdict_get() FAILED!\n");
            goto error;
        }
    }

    for (i = 28; i < 32; i++) {
        test_bucket* bucket = (test_bucket*)sad_sdict_get(&map, &buckets[i].key);

        if (bucket) {
            printf("ERROR: sad_sdict_get() FAILED!\n");
            printf("key: %i\n", buckets[i].key);
            goto error;
        }
    }

    for (i = 0; i < 28; i += 2) {
        test_bucket bucket;
        sad_int idx = sad_sdict_pop(&map, &buckets[i].key, &bucket);

        if (idx == SAD_INVALID_INDEX) {
            printf("ERROR: sad_sdict_pop() FAILED!\n");
            printf("idx: %i, key: %i\n", i, buckets[i].key);
            goto error;
        }

        if (bucket.key != buckets[i].key || strcmp(bucket.value, buckets[i].value)) {
            printf("ERROR: sad_sdict_pop() FAILED!\n");
            goto error;
        }
    }

    for (i = 0; i < 28; i += 2) {
        test_bucket* bucket = (test_bucket*)sad_sdict_get(&map, &buckets[i].key);

        if (bucket) {
            printf("ERROR: sad_sdict_pop() or sad_sdict_get() FAILED!\n");
            printf("idx: %i, key: %i\n", i, buckets[i].key);
            goto error;
        }
    }

    for (i = 1; i < 28; i += 2) {
        test_bucket* bucket = (test_bucket*)sad_sdict_get(&map, &buckets[i].key);

        if (!bucket) {
            printf("ERROR: sad_sdict_get() FAILED!\n");
            printf("key: %i\n", buckets[i].key);
            goto error;
        }
    }

    sad_sdict_destroy(&map);
    return SAD_OK;

error:
    sad_sdict_destroy(&map);
    return SAD_ERROR;
}

#endif