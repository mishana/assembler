//
// Created by misha on 30/07/2022.
//

#include "str_utils.h"
#include "errors.h"

#include <string.h>
#include <ctype.h>
#include <malloc.h>


/**
 * It checks if the string ends with the suffix.
 *
 * @param str The string to check
 * @param suffix The suffix string to be checked.
 */
bool strEndsWith(const char *str, const char *suffix) {
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);

    if (str_len < suffix_len)
        return false;

    return strcmp(str + (str_len - suffix_len), suffix) == 0;
}

/**
 * It checks if the string starts with the prefix.
 *
 * @param str The string to check
 * @param prefix The prefix to check for.
 * @param ignore_leading_whitespace If true, the function will ignore leading whitespace in the string.
 */
bool strStartsWith(const char *str, const char *prefix, bool ignore_leading_whitespace) {
    size_t str_len = strlen(str);
    size_t prefix_len = strlen(prefix);

    if (str_len < prefix_len)
        return false;

    int start_substring_from = 0;
    if (ignore_leading_whitespace) {
        start_substring_from = strFindNextNonWhitespace(str, 0);
    }

    for (size_t i = 0; i < prefix_len; ++i) {
        if (str[start_substring_from + i] != prefix[i])
            return false;
    }
    return true;
}

/**
 * Find the next whitespace character in the string, starting at the given index
 *
 * @param str The string to search in
 * @param from_idx The index to start searching from.
 */
size_t strFindNextWhitespace(const char *str, size_t from_idx) {
    while (from_idx < strlen(str) && !isspace(str[from_idx])) {
        from_idx++;
    }

    if (from_idx == strlen(str))
        return -1;
    else
        return from_idx;
}

/**
 * Find the next non-whitespace character in the string, starting at the given index
 *
 * @param str The string to search in
 * @param from_idx The index to start searching from.
 */
size_t strFindNextNonWhitespace(const char *str, size_t from_idx) {
    while (from_idx < strlen(str) && isspace(str[from_idx])) {
        from_idx++;
    }

    if (from_idx == strlen(str))
        return -1;
    else
        return from_idx;
}

/**
 * It copies at most n characters from the string s into a newly allocated string, and returns a pointer to it
 *
 * @param s The string to be copied.
 * @param n The number of characters to copy.
 */
char *strndup(const char *s, size_t n) {
    char *p;
    size_t n1;

    for (n1 = 0; n1 < n && s[n1] != '\0'; n1++)
        continue;
    p = (char *) malloc(n + 1);
    if (p != NULL) {
        memcpy(p, s, n1);
        p[n1] = '\0';
    } else {
        memoryAllocationError();
    }
    return p;
}

char *strReplace(const char *s, const char *old_substr, const char *new_substr) {
    // TODO
    return NULL;
}

List strSplit(const char *s, const char *delim) {
    // TODO
    return NULL;
}