# ifndef BSTRING_H
# define BSTRING_H

struct Bstring {
    char *data;
    size_t length;
    size_t capacity;
}

struct Bstring *bstring_init(size_t capacity, const char *const s);


# endif
