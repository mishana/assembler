//
// Created by misha on 17/08/2022.
//

#ifndef ASSEMBLER_FILE_UTILS_H
#define ASSEMBLER_FILE_UTILS_H

#include <stdio.h>

FILE *openFileWithSuffix(const char *filename, const char *mode, const char *suffix);

void removeFileWithSuffix(const char *filename, const char *suffix);

#endif //ASSEMBLER_FILE_UTILS_H
