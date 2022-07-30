//
// Created by misha on 30/07/2022.
//

#ifndef ASSEMBLER_STR_UTILS_H
#define ASSEMBLER_STR_UTILS_H

#include <stdbool.h>
#include <stdlib.h>


bool endsWith(const char *str, const char *suffix);

bool startsWith(const char *str, const char *prefix, bool ignore_leading_whitespace);

int findNextWhitespace(const char *str, int from_idx);

int findNextNonWhitespace(const char *str, int from_idx);

char *strndup(const char *s, size_t n);


#endif //ASSEMBLER_STR_UTILS_H
