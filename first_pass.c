//
// Created by misha on 27/07/2022.
//

#include "first_pass.h"
#include "parser.h"

#define SOURCE_FILE_SUFFIX ".as"
#define AFTER_MACRO_SUFFIX ".am"

#define MAX_LINE_LEN 2048


void run_first_pass(const char *filename) {
//    char filename_with_suffix[MAX_FILENAME_LEN];
//
//    concat_filename_and_suffix(filename_with_suffix, filename, SOURCE_FILE_SUFFIX);
//    FILE *src_file = fopen(filename_with_suffix, "r");
//
//    concat_filename_and_suffix(filename_with_suffix, filename, AFTER_MACRO_SUFFIX);
//    FILE *dst_file = fopen(filename_with_suffix, "w");
//
//    unfold_macros(src_file, dst_file);
//
//    fclose(src_file);
//    fclose(dst_file);
}