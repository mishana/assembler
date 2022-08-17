//
// Created by misha on 27/07/2022.
//

#include "first_pass.h"
#include "parser.h"
#include "str_utils.h"
#include "errors.h"
#include "symtab.h"
#include "const_tables.h"
#include "file_utils.h"

#include <stdio.h>

#define SOURCE_FILE_SUFFIX ".am"
#define OBJECT_FILE_SUFFIX ".ob"
#define ENTRIES_FILE_SUFFIX ".ent"
#define EXTERNAL_FILE_SUFFIX ".ext"

#define MAX_LINE_LEN 2048


/**
 * The function builds the symbol table.
 *
 * @param src_file The source file.
 * @param obj_file The file to write the object code to.
 * @param entries_file a file to write the entry symbols to.
 * @param external_file a file to write the external symbols to.
 */
List buildSymbolTable(FILE *src_file, FILE *obj_file, FILE *entries_file, FILE *external_file) {
    List symtab = listCreate((list_eq) symtabEntryCmp, (list_copy) symtabEntryCopy, (list_free) symtabEntryDestroy);

    int ic = 0, dc = 0;
    bool is_label = false;

    bool success = true;

    int line_num = 0;
    char line[MAX_LINE_LEN];
    while (fgets(line, MAX_LINE_LEN, src_file) != NULL) {
        line_num++;
        Statement s = parse(line, line_num);
        if (!statementCheckSyntax(s)) {
            continue;
        }

        if (statementGetLabel(s)) {
            is_label = true;

        }

    }
    return success ? symtab : NULL;
}

/**
 * It runs the first pass of the assembler.
 *
 * @param filename The name of the file to be read.
 */
List run_first_pass(const char *filename) {
    FILE *src_file = openFileWithSuffix(filename, "r", SOURCE_FILE_SUFFIX);

    FILE *entries_file = openFileWithSuffix(filename, "w", ENTRIES_FILE_SUFFIX);
    FILE *external_file = openFileWithSuffix(filename, "w", EXTERNAL_FILE_SUFFIX);
    FILE *object_file = openFileWithSuffix(filename, "w", OBJECT_FILE_SUFFIX);

    List symtab = buildSymbolTable(src_file, object_file, entries_file, external_file);

    fclose(src_file);
    fclose(entries_file);
    fclose(external_file);
    fclose(object_file);
}
