#include "types_utils.h"
#include <ctype.h>
#include <string.h>

/**
 * It checks if the string is a string.
 *
 * @param str The string to check
 */
bool isString(const char *str) {
    return str[0] == '\"' && str[strlen(str) - 1] == '\"';
}

/**
 * It checks if the string is alphabetic.
 *
 * @param str The string to check
 */
bool isAlpha(const char *str) {
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        if (!isalpha(str[i])) {
            return false;
        }
    }
    return true;
}

/**
 * It checks if the string is a number.
 *
 * @param str The string to check
 */
bool isNumeric(const char *str) {
    int i = 0;
    if (str[0] == '-' || str[0] == '+') {
        i = 1;
    }

    for (; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

/**
 * It checks if the string contains only letters and digits.
 *
 * @param str The string to check
 */
bool isAlphaNumeric(const char *str) {
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        if (!isalnum(str[i])) {
            return false;
        }
    }
    return true;
}