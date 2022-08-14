#include <assert.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include "str.h"

static const size_t STR_DEFAULT_SIZE = 8;
static const size_t STR_RESIZE_SIZE = 32;

struct str {
    char* data;
    size_t used;
    size_t max;
};

/* -- Private Interface -- */

static bool is_full(struct str* self) {
    assert(self != (void*) 0);
    assert(self->data != (void*) 0);

    if (self->used >= self->max) {
        return true;
    }

    return false;
}

static bool resize(struct str* self, size_t value) {
    assert(self != (void*) 0);
    assert(self->data != (void*) 0);
    assert(self->used + value >= self->max);

    if (!value) {
        value = STR_RESIZE_SIZE;
    }

    /* overflow */
    if (self->max + value < self->max) {
        return false;
    }

    self->max += value;

    char* data = realloc(self->data, self->max);

    if (!data) {
        self->max -= value;
        return false;
    }

    self->data = data;

    return true;
}

/* -- Public Interface Implementation -- */

struct str* str_new(void) {
    struct str* str = malloc(sizeof (struct str));

    if (!str) {
        return (void*) 0;
    }

    str->used = 0;
    str->max = STR_DEFAULT_SIZE;

    str->data = malloc(str->max);

    if (!str->data) {
        free(str);
        return (void*) 0;
    }

    return str;
}

void str_del(str* self) {
    if (!self) {
        return;
    }

    if (self->data) {
        free(self->data);
    }

    free(self);
}

struct str* str_from_char(char c) {
    struct str* s = str_new();

    if (!s) {
        return (void*) 0;
    }

    if (!str_append_char(s, c)) {
        str_del(s);
        return (void*) 0;
    }

    return s;
}

struct str* str_from_cstr(char const* s) {
    struct str* snew = str_new();

    if (!snew) {
        return (void*) 0;
    }

    if (!s) {
        return snew;
    }

    if (!str_append_cstr(snew, s)) {
        str_del(snew);
        return (void*) 0;
    }

    return snew;
}

struct str* str_from_str(struct str* s) {
    return str_clone(s);
}

char const* str_cstr(struct str* self) {
    if (!self) {
        return "";
    }

    assert(self->data != (void*) 0);

    /* makes sure cstr is null terminated */
    self->data[self->used] = 0;

    return self->data;
}

size_t str_len(struct str* self) {
    if (!self) {
        return 0;
    }

    assert(self->data != (void*) 0);

    return self->used;
}

bool str_empty(struct str* self) {
    if (!self) {
        return false;
    }

    assert(self->data != (void*) 0);

    return self->used == 0;
}

bool str_append_char(struct str* self, char c) {
    if (!self) {
        return false;
    }

    assert(self->data != (void*) 0);

    if (is_full(self)) {
        if (!resize(self, 0)) {
            return false;
        }
    }

    self->data[self->used] = c;
    self->used++;

    return true;
}

bool str_append_cstr(struct str* self, char const* s) {
    if (!self || !s) {
        return false;
    }

    for (char const* c = s; *c; ++c) {
        if (!str_append_char(self, *c)) return false;
    }

    return true;
}

bool str_append_str(struct str* self, struct str* s) {
    if (!self || !s) {
        return false;
    }

    assert(self->data != (void*) 0);
    assert(s->data != (void*) 0);

    size_t new_used = self->used + s->used;

    /* overflow */
    if (new_used < self->used) {
        return false;
    }

    if (new_used > self->max) {
        if (!resize(self, new_used)) {
            return false;
        }
    }

    for (size_t i = 0; i < s->used; ++i) {
        self->data[self->used] = s->data[i];
        self->used++;
    }

    return true;
}

bool str_clear(struct str* self) {
    if (!self) {
        return false;
    }

    assert(self->data != (void*) 0);

    return str_remove(self, 0, self->used);
}

void str_reverse(str* self) {
    if (!self) {
        return;
    }

    assert(self->data != (void*) 0);

    if (self->used < 1) {
        return;
    }

    /* This takes advantage of the XOR Operation's property.
     * For reference, visit: https://en.wikipedia.org/wiki/XOR_swap_algorithm */
    for (size_t i = 0, j = self->used - 1; i < j; ++i, --j) {
        self->data[i] = self->data[i] ^ self->data[j];
        self->data[j] = self->data[j] ^ self->data[i];
        self->data[i] = self->data[i] ^ self->data[j];
    }
}

bool str_remove(str* self, size_t start, size_t end) {
    if (!self) {
        return false;
    }

    if (start > self->used) {
        /* there is nothing to remove */
        return true;
    }

    if (!end || end > self->used) {
        /* we should truncate to the end of the string */
        end = self->used;
    }

    size_t delta = end - start;

    assert(start + delta <= self->used);

    if (start + delta == self->used) {
        self->used -= delta;

        /* + 1 for the \0 byte */
        self->max = self->used + 1;
        self->data[start] = 0;

        char* new_data = realloc(self->data, self->max);

        if (!new_data) {
            return false;
        }

        self->data = new_data;

        return true;
    }

    memmove(self->data + start, self->data + end, self->used - end);

    self->used -= delta;

    /* + 1 for the \0 byte */
    self->max = self->used + 1;
    self->data[self->max] = 0;

    char* new_data = realloc(self->data, self->max);

    if (!new_data) {
        return false;
    }

    self->data = new_data;

    return true;
}

struct str* str_slice(struct str* self, size_t start, size_t end) {
    if (!self) {
        return (void*) 0;
    }

    if (start > self->used) {
        return str_new();
    }

    if (!end || end > self->used) {
        end = self->used;
    }

    struct str* s = str_new();

    for (; start < end; start++) {
        if (!str_append_char(s, self->data[start])) {
            str_del(s);
            return (void*) 0;
        }
    }

    return s;
}

struct str* str_clone(struct str* self) {
    if (!self) {
        return str_new();
    }

    assert(self->data != (void*) 0);

    return str_slice(self, 0, str_len(self));
}

int str_cmp(struct str* s1, struct str* s2) {
    if (!s1 || !s2) {
        return INT_MIN;
    }

    assert(s1->data != (void*) 0);
    assert(s2->data != (void*) 0);

    return strcmp(str_cstr(s1), str_cstr(s2));
}

bool str_equal(struct str* s1, struct str* s2) {
    if (!s1 || !s2) {
        return false;
    }

    if (str_len(s1) != str_len(s2)) {
        return false;
    }

    assert(s1->data != (void*) 0);
    assert(s2->data != (void*) 0);

    return memcmp(s1->data, s2->data, str_len(s1)) == 0;
}
