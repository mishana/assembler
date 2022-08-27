#ifndef ASSEMBLER_STR_UTILS_H
#define ASSEMBLER_STR_UTILS_H

#include <stdbool.h>
#include <stdlib.h>
#include "linkedlist.h"


bool strEndsWith(const char *str, const char *suffix);

bool strStartsWith(const char *str, const char *prefix, bool ignore_leading_whitespace);

size_t strFindNextWhitespace(const char *str, size_t from_idx);

size_t strFindNextNonWhitespace(const char *str, size_t from_idx);

size_t strFindNextNonWhitespaceAndDelimiter(const char *str, size_t from_idx, char delim);

size_t strFindNextWhitespaceOrDelimiter(const char *str, size_t from_idx, char delim);

size_t strFindNextChar(const char *str, size_t from_idx, char c);

char *strndup(const char *s, size_t n);

char *strdup(const char *s);

List strSplit(const char *s, const char *delim);

char *strConcat(const char *s1, const char *s2);


int strCountCharInRange(const char *s, char c, int start_idx, int stop_idx);

#endif
