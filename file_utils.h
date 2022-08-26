//
// Created by misha on 17/08/2022.
//

#ifndef ASSEMBLER_FILE_UTILS_H
#define ASSEMBLER_FILE_UTILS_H

#include <stdio.h>

#define MAX_LINE_LEN 80 + 1 + 1 // +1 for '\n' and +1 for '\0'
#define LINE_BUFFER_LEN MAX_LINE_LEN + 1


FILE *openFileWithSuffix(const char *filename, const char *mode, const char *suffix);

void removeFileWithSuffix(const char *filename, const char *suffix);

#endif //ASSEMBLER_FILE_UTILS_H
