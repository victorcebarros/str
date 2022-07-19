#include <stdio.h>
#include <stdlib.h>

#include "str.h"

void is_palindrome(char const* s) {
    str* word = str_from(s);
    str* reversed = str_clone(word);

    str_reverse(reversed);

    printf("%s is %sa palindrome!\n",
            str_cstr(word),
            str_equal(word, reversed)
                ? ""
                : "not ");

    str_del(reversed);
    str_del(word);
}

int main(int argc, char const** argv) {
    if (argc < 2) {
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; ++i) {
        is_palindrome(argv[i]);
    }

    return EXIT_SUCCESS;
}
