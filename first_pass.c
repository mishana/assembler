//
// Created by misha on 27/07/2022.
//

#include "first_pass.h"
#include "parser.h"
#include "str_utils.h"
#include "errors.h"

#include <stdio.h>

#define SOURCE_FILE_SUFFIX ".as"
#define AFTER_MACRO_SUFFIX ".am"

#define MAX_LINE_LEN 2048


void run_first_pass(const char *filename) {
//    const char *filename_with_suffix = strConcat(filename, SOURCE_FILE_SUFFIX);
//    FILE *src_file = fopen(filename_with_suffix, "r");
//    if (!src_file) {
//        fileNotFoundError(filename_with_suffix);
//    }
//    free((void *) filename_with_suffix);
//
//    // TODO: different file extension for the output file
//    filename_with_suffix = strConcat(filename, AFTER_MACRO_SUFFIX);
//    FILE *dst_file = fopen(filename_with_suffix, "w");
//    if (!dst_file) {
//        fileNotFoundError(filename_with_suffix);
//    }
//    free((void *) filename_with_suffix);
//
//    // TODO: logic here
//
//    fclose(src_file);
//    fclose(dst_file);
}