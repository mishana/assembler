#include "str_utils.h"
#include "errors.h"

#include <string.h>
#include <ctype.h>
#include <malloc.h>
#include <assert.h>


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
    int start_substring_from = 0;
    size_t i;

    if (str_len < prefix_len)
        return false;

    if (ignore_leading_whitespace) {
        start_substring_from = strFindNextNonWhitespace(str, 0);
    }

    for (i = 0; i < prefix_len; ++i) {
        if (strlen(str) > start_substring_from + i && str[start_substring_from + i] != prefix[i])
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
 * Find the next non-whitespace character that is not the delimiter.
 *
 * @param str The string to search in.
 * @param from_idx The index to start searching from.
 * @param delim The delimiter to look for.
 */
size_t strFindNextNonWhitespaceAndDelimiter(const char *str, size_t from_idx, char delim) {
    while (from_idx < strlen(str) && (isspace(str[from_idx]) || str[from_idx] == delim)) {
        from_idx++;
    }

    if (from_idx == strlen(str))
        return -1;
    else
        return from_idx;
}

/**
 * Find the next whitespace or delimiter character in a string.
 *
 * @param str The string to search in.
 * @param from_idx The index to start searching from.
 * @param delim The delimiter to look for.
 */
size_t strFindNextWhitespaceOrDelimiter(const char *str, size_t from_idx, char delim) {
    while (from_idx < strlen(str) && !(isspace(str[from_idx]) || str[from_idx] == delim)) {
        from_idx++;
    }

    if (from_idx == strlen(str))
        return -1;
    else
        return from_idx;
}

/**
 * Find the next occurrence of a character in a string.
 *
 * @param str The string to search in.
 * @param from_idx The index to start searching from.
 * @param c The character to search for.
 */
size_t strFindNextChar(const char *str, size_t from_idx, char c) {
    while (from_idx < strlen(str) && str[from_idx] != c) {
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


/**
 * It returns a pointer to a copy of the string s.
 *
 * @param s The string to be duplicated.
 */
char *strdup(const char *s) {
    size_t size;
    char *p;

    if (!s) {
        return NULL;
    }
    size = strlen(s) + 1;
    p = malloc(size);
    if (p != NULL) {
        memcpy(p, s, size);
    }
    return p;
}

/**
 * It splits a string by delimiters into a list of strings.
 *
 * @param s The string to split.
 * @param delim a string of delimiters.
 */
List strSplit(const char *s, const char *delim) {
    List l = listCreate((list_eq) strcmp, (list_copy) strdup, free);
    char *tmp = strdup(s);
    char *token;

    for (token = strtok(tmp, delim); token; token = strtok(NULL, delim)) {
        listAppend(l, token);
    }

    free(tmp);
    return l;
}

/**
 * It concatenates two strings.
 *
 * @param s1 The first string to be concatenated.
 * @param s2 The string to be appended to s1.
 */
char *strConcat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    if (result) {
        strcpy(result, s1);
        strcat(result, s2);
    } else {
        memoryAllocationError();
    }
    return result;
}

/**
 * Count the number of times a character appears in a string between two indices.
 *
 * @param s The string to search in.
 * @param c the character to count
 * @param start_idx the index of the first character to check
 * @param stop_idx the index of the last character to be counted
 */
int strCountCharInRange(const char *s, char c, int start_idx, int stop_idx) {
    int count = 0;
    int i = start_idx;

    while (i < stop_idx) {
        if (strlen(s) > i && s[i] == c) {
            count++;
        }
        i++;
    }
    return count;
}
