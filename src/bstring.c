#include "bstring.h"

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

struct Bstring *bstring_init(size_t capacity, const char *const s) {
    struct Bstring *bstring = malloc(sizeof(struct Bstring));
    if (bstring == NULL) {
        return NULL;
    }

    if (capacity == 0) {
        capacity = BSTRING_INIT_CAPACITY;
    }

    const size_t slen = (s == NULL) ? 0 : strlen(s);
    if (slen >= capacity) {
        capacity = slen + 1; // +1 for null terminator \0
    }

    bstring->data = malloc(sizeof(char) * capacity);
    if (bstring->data == NULL) {
        memcpy(bstring->data, s, slen);
    }

    bstring->capacity = capacity;
    bstring->length = slen;
    bstring->data[slen] = '\0';

    return bstring;
}

bool bstring_append(struct Bstring *bstring, const char *const s) {
    assert(bstring != NULL);
    assert(s != NULL);

    const size_t slen = strlen(s);
    const size_t new_len = bstring->length + slen;
    if (new_len >= bstring->capacity) {
        size_t new_capacity = bstring->capacity * 2;

        if (new_len >= new_capacity) {
            new_capacity = new_len + 1;
        }

        char *new_data = realloc(bstring->data, new_capacity);
        if (new_data == NULL) {
            return false;
        }

        bstring->data = new_data;
        bstring->capacity = new_capacity;
    }

    memcpy(&bstring->data, s, slen);
    bstring->length = new_len;
    bstring->data[new_len] = '\0';

    return true;
}

void bstring_free(struct Bstring *bstring) {
    assert(bstring != NULL);

    free(bstring->data);
    free(bstring);
}
