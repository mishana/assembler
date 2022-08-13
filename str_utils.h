//
// Created by misha on 30/07/2022.
//

#ifndef ASSEMBLER_STR_UTILS_H
#define ASSEMBLER_STR_UTILS_H

#include <stdbool.h>
#include <stdlib.h>
#include "linkedlist.h"


bool strEndsWith(const char *str, const char *suffix);

bool strStartsWith(const char *str, const char *prefix, bool ignore_leading_whitespace);

size_t strFindNextWhitespace(const char *str, size_t from_idx);

size_t strFindNextNonWhitespace(const char *str, size_t from_idx);

char *strndup(const char *s, size_t n);

char *strReplace(const char *s, const char *old_substr, const char *new_substr);

List strSplit(const char *s, const char *delim);

char *strConcat(const char *s1, const char *s2);


#endif //ASSEMBLER_STR_UTILS_H
