# ifndef BSTRING_H
# define BSTRING_H
#include <stdlib.h>
#include <stdbool.h>

#define BSTRING_INIT_CAPACITY 16

struct Bstring {
    char *data;
    size_t length;
    size_t capacity;
};

struct Bstring *bstring_init(size_t capacity, const char *const s);
bool bstring_append(struct Bstring *bstring, const char *const s);
void bstring_free(struct Bstring *bstring);

# endif
