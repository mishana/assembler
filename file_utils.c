//
// Created by misha on 17/08/2022.
//

#include <stdio.h>
#include "file_utils.h"
#include "str_utils.h"
#include "errors.h"


/**
 * It opens a file with a suffix.
 *
 * @param filename The name of the file to open.
 * @param mode "r" for read, "w" for write, "a" for append, "r+" for read and write, "w+" for write and read, "a+" for
 * append and read
 * @param suffix The suffix to append to the filename.
 */
FILE *openFileWithSuffix(const char *filename, const char *mode, const char *suffix) {
    const char *filename_with_suffix = strConcat(filename, suffix);
    FILE *file = fopen(filename_with_suffix, mode);
    if (!file) {
        fileNotFoundError(filename_with_suffix);
    }
    free((void *) filename_with_suffix);
    return file;
}

/**
 * It removes the file with the given suffix.
 *
 * @param filename The name of the file to remove.
 * @param suffix The suffix of the file to be removed.
 */
void removeFileWithSuffix(const char *filename, const char *suffix) {
    const char *filename_with_suffix = strConcat(filename, suffix);
    remove(filename_with_suffix);
    free((void *) filename_with_suffix);
}
