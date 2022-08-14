#include <assert.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <string.h>

#include <cmocka.h>

#include "str.h"

static void str_new_del_test(void** state) {
    (void) state;

    str* s = str_new();
    str_del(s);
}

static void str_cstr_test(void** state) {
    (void) state;

    str* s = str_new();

    assert_string_equal(str_cstr(s), "");

    str_del(s);
}

static void str_len_test(void** state) {
    (void) state;

    str* s = str_new();

    assert(str_len(s) == 0);

    str_del(s);
}

static void str_append_char_literal_test(void** state) {
    (void) state;

    str* s = str_new();

    for (int i = 0; i < 32; ++i) {
        str_append(s, 'a');
    }

    assert_string_equal(str_cstr(s), "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");

    str_del(s);
}

static void str_append_cstr_null_test(void** state) {
    (void) state;

    str* s = str_new();

    str_append(s, (char*) 0);

    assert_string_equal(str_cstr(s), "");

    str_del(s);
}

static void str_append_cstr_literal_test(void** state) {
    (void) state;

    str* s = str_new();
    char const* cstr = "Hello brilliant world! Impressive :)";

    str_append(s, cstr);

    assert_string_equal(str_cstr(s), cstr);
    assert(str_len(s) == strlen(cstr));

    str_del(s);
}

static void str_append_str_null_test(void** state) {
    (void) state;

    str* s = str_new();

    str_append(s, (str*) 0);

    assert_string_equal(str_cstr(s), "");

    str_del(s);
}

static void str_append_str_test(void** state) {
    (void) state;

    str* a = str_new();
    str* s = str_new();

    char const* cstr = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa";

    str_append(a, cstr);
    str_append(s, a);

    assert_string_equal(str_cstr(a), str_cstr(s));
    assert_string_equal(str_cstr(s), cstr);

    assert(str_len(a) == str_len(s));
    assert(str_len(s) == strlen(cstr));

    str_del(a);
    str_del(s);
}

static void str_append_cstr_empty_test(void** state) {
    (void) state;

    str* s = str_new();
    str_append(s, "");

    assert_string_equal(str_cstr(s), "");
    assert(str_len(s) == 0);

    str_del(s);
}

static void str_append_str_empty_test(void** state) {
    (void) state;

    str* a = str_new();
    str* s = str_new();

    str_append(s, "");
    str_append(a, s);

    assert_string_equal(str_cstr(s), str_cstr(s));
    assert(str_len(a) == str_len(s));

    str_del(a);
    str_del(s);
}

static void str_clear_test(void** state) {
    (void) state;

    str* s = str_new();

    str_append(s, "Hello there!");

    str_clear(s);

    assert_string_equal(str_cstr(s), "");
    assert(str_len(s) == 0);

    str_del(s);
}

static void str_reverse_test(void** state) {
    (void) state;

    str* s = str_new();

    char const* original = "qwertyuiopasdfghjkl;'\\zxcvbnm,./1234567890-=";
    char const* reversed = "=-0987654321/.,mnbvcxz\\';lkjhgfdsapoiuytrewq";

    str_append(s, original);

    str_reverse(s);

    assert_string_equal(str_cstr(s), reversed);

    str_del(s);
}

static void str_remove_beginning_test(void** state) {
    (void) state;

    str* s = str_new();
    str_append(s, "Good, good, bad, good...");

    str_remove(s, 0, 6);
    assert_string_equal(str_cstr(s), "good, bad, good...");
    assert(str_len(s) == 18);
    assert_null(str_cstr(s)[str_len(s)]);

    str_del(s);
}

static void str_remove_middle_test(void** state) {
    (void) state;

    str* s = str_new();
    str_append(s, "Good, good, bad, good...");

    str_remove(s, 12, 17);
    assert_string_equal(str_cstr(s), "Good, good, good...");
    assert(str_len(s) == 19);
    assert_null(str_cstr(s)[str_len(s)]);

    str_del(s);
}

static void str_remove_end_test(void** state) {
    (void) state;

    str* s = str_new();
    str* s2 = str_new();
    str* s3 = str_new();
    str_append(s, "Good, good, bad, good...");
    str_append(s2, "Good, good, bad, good...");
    str_append(s3, "Good, good, bad, good...");

    /* If end == 0, consider all the way to the end */
    str_remove(s, 21, 0);
    str_remove(s2, 21, str_len(s2));

    /* If end > str_len(s), consider to the end of the string */
    str_remove(s3, 21, 41234);

    assert_string_equal(str_cstr(s), "Good, good, bad, good");
    assert_string_equal(str_cstr(s2), "Good, good, bad, good");
    assert_string_equal(str_cstr(s3), "Good, good, bad, good");

    assert(str_len(s) == 21);
    assert(str_len(s2) == 21);
    assert(str_len(s3) == 21);

    assert_null(str_cstr(s)[str_len(s)]);
    assert_null(str_cstr(s2)[str_len(s2)]);
    assert_null(str_cstr(s3)[str_len(s3)]);

    str_del(s);
    str_del(s2);
    str_del(s3);
}

static void str_slice_beginning_test(void** state) {
    (void) state;

    char const* cstr = "Abraham Lincoln: Whatever you are, be a good one.";

    str* s = str_new();

    str_append(s, cstr);

    str* sliced = str_slice(s, 0, 15);

    assert_string_equal(str_cstr(sliced), "Abraham Lincoln");

    str_del(s);
    str_del(sliced);
}

static void str_slice_middle_test(void** state) {
    (void) state;

    char const* cstr = "Abraham Lincoln: Whatever you are, be a good one.";

    str* s = str_new();

    str_append(s, cstr);

    str* sliced = str_slice(s, 17, 33);

    assert_string_equal(str_cstr(sliced), "Whatever you are");

    str_del(s);
    str_del(sliced);
}

static void str_slice_end_test(void** state) {
    (void) state;

    char const* cstr = "Abraham Lincoln: Whatever you are, be a good one.";

    str* s = str_new();

    str_append(s, cstr);

    /* If end == 0, consider all the way to the end */
    str* sliced = str_slice(s, 35, 0);
    str* sliced2 = str_slice(s, 35, str_len(s));

    /* If end > str_len(s), consider to the end of the string */
    str* sliced3 = str_slice(s, 35, 41234);

    assert_string_equal(str_cstr(sliced), "be a good one.");
    assert_string_equal(str_cstr(sliced2), "be a good one.");
    assert_string_equal(str_cstr(sliced3), "be a good one.");

    str_del(s);
    str_del(sliced);
    str_del(sliced2);
    str_del(sliced3);
}

static void str_empty_test(void** state) {
    (void) state;

    str* s = str_new();

    assert_true(str_empty(s));

    str_append(s, "qwerty");

    assert_false(str_empty(s));

    str_del(s);
}

static void str_clone_test(void** state) {
    (void) state;

    str* s = str_new();
    str_append(s, "abcdefghijklmnopqrstuvwxyz0123456789");

    str* a = str_clone(s);

    assert_string_equal(str_cstr(s), str_cstr(a));
    assert(str_len(s) == str_len(a));

    str_del(s);
    str_del(a);
}

static void str_cmp_eq_test(void** state) {
    (void) state;

    str* s1 = str_new();
    str* s2 = str_new();

    /* these two strings are compared loosely, meaning
     * are equal despite s1 being bigger than s2. */
    str_append(s1, "hello there!\0hidden text!");
    str_append(s2, "hello there!");

    assert_int_equal(str_cmp(s1, s2), 0);

    str_del(s1);
    str_del(s2);
}

static void str_cmp_ne_test(void** state) {
    (void) state;

    str* s1 = str_new();
    str* s2 = str_new();
    str_append(s1, "hallo!");
    str_append(s2, "hello!");

    assert_int_not_equal(str_cmp(s1, s2), 0);

    str_del(s1);
    str_del(s2);
}

static void str_equal_eq_test(void** state) {
    (void) state;

    str* s1 = str_new();
    str* s2 = str_new();
    str_append(s1, "hello there!");
    str_append(s2, "hello there!");

    assert_true(str_equal(s1, s2));

    str_del(s1);
    str_del(s2);
}

static void str_equal_ne_test(void** state) {
    (void) state;

    str* s1 = str_new();
    str* s2 = str_new();
    str_append(s1, "sehr beeindruckend!");
    str_append(s2, "very impressive!");

    assert_false(str_equal(s1, s2));

    str_del(s1);
    str_del(s2);
}

static void str_from_char_literal_test(void** state) {
    (void) state;

    str* s = str_from('a');

    assert_string_equal(str_cstr(s), "a");
    assert(str_len(s) == 1);

    str_del(s);
}

static void str_from_cstr_null_test(void** state) {
    (void) state;

    str* s = str_from((char*) 0);

    assert_string_equal(str_cstr(s), "");

    str_del(s);
}

static void str_from_cstr_literal_test(void** state) {
    (void) state;

    str* s = str_from("abcdefghijklmnopqrstuvwxyz0123456789");

    assert_string_equal(str_cstr(s), "abcdefghijklmnopqrstuvwxyz0123456789");
    assert(str_len(s) == 36);

    str_del(s);
}

static void str_from_str_null_test(void** state) {
    (void) state;

    str* s = str_from((str*) 0);

    assert_string_equal(str_cstr(s), "");

    str_del(s);
}

static void str_from_str_test(void** state) {
    (void) state;

    char const* cstr = "the room echoes the sound of your voice";

    str* s = str_new();
    str_append(s, cstr);

    str* s1 = str_from(s);

    assert_string_equal(str_cstr(s1), cstr);
    assert(str_len(s1) == strlen(cstr));

    str_del(s1);
    str_del(s);
}

static void str_from_cstr_empty_test(void** state) {
    (void) state;

    str* s = str_from("");

    assert_string_equal(str_cstr(s), "");
    assert(str_len(s) == 0);

    str_del(s);
}

static void str_from_str_empty_test(void** state) {
    (void) state;

    str* s = str_new();
    str* s1 = str_from(s);

    assert_string_equal(str_cstr(s), "");
    assert(str_len(s) == 0);

    str_del(s1);
    str_del(s);
}

int main(void) {
    struct CMUnitTest const tests[] = {
        cmocka_unit_test(str_new_del_test),
        cmocka_unit_test(str_cstr_test),
        cmocka_unit_test(str_len_test),
        cmocka_unit_test(str_append_char_literal_test),
        cmocka_unit_test(str_append_cstr_null_test),
        cmocka_unit_test(str_append_cstr_literal_test),
        cmocka_unit_test(str_append_str_null_test),
        cmocka_unit_test(str_append_str_test),
        cmocka_unit_test(str_append_cstr_empty_test),
        cmocka_unit_test(str_append_str_empty_test),
        cmocka_unit_test(str_clear_test),
        cmocka_unit_test(str_reverse_test),
        cmocka_unit_test(str_remove_beginning_test),
        cmocka_unit_test(str_remove_middle_test),
        cmocka_unit_test(str_remove_end_test),
        cmocka_unit_test(str_slice_beginning_test),
        cmocka_unit_test(str_slice_middle_test),
        cmocka_unit_test(str_slice_end_test),
        cmocka_unit_test(str_empty_test),
        cmocka_unit_test(str_clone_test),
        cmocka_unit_test(str_cmp_eq_test),
        cmocka_unit_test(str_cmp_ne_test),
        cmocka_unit_test(str_equal_eq_test),
        cmocka_unit_test(str_equal_ne_test),
        cmocka_unit_test(str_from_char_literal_test),
        cmocka_unit_test(str_from_cstr_literal_test),
        cmocka_unit_test(str_from_str_test),
        cmocka_unit_test(str_from_cstr_null_test),
        cmocka_unit_test(str_from_cstr_empty_test),
        cmocka_unit_test(str_from_str_null_test),
        cmocka_unit_test(str_from_str_empty_test),
    };


    return cmocka_run_group_tests(tests, (void*) 0, (void*) 0);
}
