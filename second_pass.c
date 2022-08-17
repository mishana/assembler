//
// Created by misha on 27/07/2022.
//

#include <stdio.h>
#include "second_pass.h"
#include "linkedlist.h"
#include "file_utils.h"

#define SOURCE_FILE_SUFFIX ".am"
#define OBJECT_FILE_SUFFIX ".ob"
#define ENTRIES_FILE_SUFFIX ".ent"
#define EXTERNAL_FILE_SUFFIX ".ext"

#define MAX_LINE_LEN 2048


void run_second_pass(const char *filename, List symtab) {
    FILE *src_file = openFileWithSuffix(filename, "r", SOURCE_FILE_SUFFIX);

    FILE *entries_file = openFileWithSuffix(filename, "w", ENTRIES_FILE_SUFFIX);
    FILE *external_file = openFileWithSuffix(filename, "w", EXTERNAL_FILE_SUFFIX);
    FILE *object_file = openFileWithSuffix(filename, "w", OBJECT_FILE_SUFFIX);

    // TODO: implement second pass

    listDestroy(symtab);

    fclose(src_file);
    fclose(entries_file);
    fclose(external_file);
    fclose(object_file);
}