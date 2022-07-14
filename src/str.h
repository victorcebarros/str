/** str's Public API
 * @file str.h */
#ifndef STR_H
#define STR_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include <stdbool.h>
#include <stddef.h>

/** Opaque str Structure */
typedef struct str str;

/** Creates empty str.
 * @warning The user has to free the object after usage with
 *          str_del.
 *
 * @return An empty str object.
 *
 * @see str_del */
str* str_new(void);

/** Deletes str.
 * @param self A pointer to a str object. */
void str_del(str* self);

/** Creates str from character.
 * @warning The user has to free the object after usage with
 *          str_del.
 *
 * @param c A character.
 *
 * @return  A pointer to a str object containing the character.
 *
 * @see str_from str_from_cstr str_from_str str_del */
str* str_from_char(char c);

/** Creates str from null terminated C string.
 * @warning The user has to free the object after usage with
 *          str_del.
 *
 * @warning The C string needs to be null terminated!
 *
 * @param s A pointer to a C string.
 *
 * @return  A pointer to a str object containing the C string.
 *
 * @see str_from str_from_char str_from_str str_del */
str* str_from_cstr(char const* s);

/** Creates str from str object.
 * @warning The user has to free the object after usage with
 *          str_del.
 *
 * @param s A pointer to a str object.
 *
 * @return  A pointer to a str object.
 *
 * @see str_from str_from_char str_from_cstr str_clone str_del */
str* str_from_str(str* s);

/** Generic for str_from_*.
 * @note       If T is a character literal, e.g. 'a',
 *             it will suffer from integral promotion in C,
 *             meaning it will be perceived as int, hence
 *             the int type is handled by str_append_char.
 *
 * @param T    Generic Object.
 *
 * @return     Newly created str object.
 *
 * @see str_from_char str_from_cstr str_from_str str_del */
#define str_from(T) _Generic((T),   \
        int:         str_from_char, \
        char:        str_from_char, \
        char*:       str_from_cstr, \
        char const*: str_from_cstr, \
        str*:        str_from_str   \
        )(T)

/** Clones the str object.
 * @warning    The user has to free the object after usage with
 *             str_del.
 *
 * @param self A pointer to a str object.
 *
 * @return     A pointer to a new cloned str.
 *
 * @see str_del */
str* str_clone(str* self);

/** Returns null terminated C string.
 *
 * @param self A pointer to a str object.
 *
 * @return     Null terminated C string.
 *
 * @see str_len */
char const* str_cstr(str* self);

/** Returns str length.
 *
 * @param self A pointer to a str object.
 *
 * @see str_empty */
size_t str_len(str* self);

/** Returns true if str is empty.
 *
 * @param self A pointer to a str object.
 *
 * @see str_len */
bool str_empty(str* self);

/** Removes all characters from str.
 *
 * @param self A pointer to a str object.
 *
 * @return     true if successful.
 *
 * @see str_remove */
bool str_clear(str* self);

/** Reverses str.
 * @param self A pointer to a str object. */
void str_reverse(str* self);

/** Appends a single character to str.
 *
 * @param self A pointer to a str object.
 * @param c    A character.
 *
 * @return     true if successful.
 *
 * @see str_append str_append_cstr str_append_str */
bool str_append_char(str* self, char c);

/** Appends a null terminated C string to str.
 * @warning    The C string needs to be null terminated!
 *
 * @param self A pointer to a str object.
 * @param s    A pointer to a character string.
 *
 * @return     true if successful.
 *
 * @see str_append str_append_char str_append_str */
bool str_append_cstr(str* self, char const* s);

/** Appends a str s to str self.
 *
 * @param self A pointer to a str object.
 * @param s    A pointer to a str object.
 *
 * @return     true if successful.
 *
 * @see str_append str_append_char str_append_cstr */
bool str_append_str(str* self, str* s);

/** Generic for str_append_*.
 * @note       If T is a character literal, e.g. 'a',
 *             it will suffer from integral promotion in C,
 *             meaning it will be perceived as int, hence
 *             the int type is handled by str_append_char.
 *
 * @param self A pointer to a str object.
 * @param T    Generic Object.
 *
 * @return     true if successful.
 *
 * @see str_append_char str_append_cstr str_append_str */
#define str_append(self, T) _Generic((T), \
        int:         str_append_char,     \
        char:        str_append_char,     \
        char*:       str_append_cstr,     \
        char const*: str_append_cstr,     \
        str*:        str_append_str       \
        )(self, T)

/** Compares two strings loosely.
 * @warning    Comparisons in this function are done in
 *             the same way as strcmp, meaning that if str A contains
 *             "qwerty\0othertext" and str B contains "qwerty\0", they
 *             will still be marked as equal. If you wish to assure
 *             strict equality between str types, use str_equal.
 *
 * @param self A pointer to a str object.
 * @param s    A pointer to a str object.
 *
 * @return     The same as strcmp would return.
 *
 * @see str_equal */
int str_cmp(str* self, str* s);

/** Compares two strings strictly.
 *
 * @param self A pointer to a str object.
 * @param s    A pointer to a str object.
 *
 * @return     true if they are strictly equal or false otherwise.
 *
 * @see str_cmp */
bool str_equal(str* self, str* s);

/** Removes characters within range from str.
 * @note        The range is closed on start and open on end,
 *              which means it includes the character pointed
 *              by start but it doesn't remove the last character
 *              indexed by end.
 *
 * @param self  A pointer to a str object.
 * @param start Start index.
 * @param end   End index.
 *
 * @return      true if successful.
 *
 * @see str_clear */
bool str_remove(str* self, size_t start, size_t end);

/** Creates new str from str slice.
 * @warning     The user has to free the object after usage with
 *              str_del.
 *
 * @note        The range is closed on start and open on end,
 *              which means it includes the character pointed
 *              by start but it doesn't remove the last character
 *              indexed by end.
 *
 * @param self  A pointer to a str object.
 * @param start Start index.
 * @param end   End index.
 *
 * @return      A pointer to a new str containing slice.
 *
 * @see str_remove str_del */
str* str_slice(str* self, size_t start, size_t end);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STR_H */
