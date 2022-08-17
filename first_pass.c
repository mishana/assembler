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
#include "memmap.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

#define SOURCE_FILE_SUFFIX ".am"
#define OBJECT_FILE_SUFFIX ".ob"
#define ENTRIES_FILE_SUFFIX ".ent"
#define EXTERNAL_FILE_SUFFIX ".ext"

#define MAX_LINE_LEN 80 + 1


/**
 * The function builds the symbol table.
 *
 * @param src_file The source file.
 * @param obj_file The file to write the object code to.
 * @param entries_file a file to write the entry symbols to.
 * @param external_file a file to write the external symbols to.
 */
List buildSymbolTable(FILE *src_file) {
    List symtab = listCreate((list_eq) symtabEntryCmp, (list_copy) symtabEntryCopy, (list_free) symtabEntryDestroy);

    size_t ic = 0, dc = 0;
    bool is_label = false;

    bool success = true;

    int line_num = 0;
    char line[MAX_LINE_LEN];
    while (fgets(line, MAX_LINE_LEN, src_file) != NULL) {
        line_num++;
        Statement s = parse(line, line_num);
        if (!s || !statementCheckSyntax(s)) {
            success = false;
            continue;
        }
        if (statementGetType(s) == EMPTY_LINE || statementGetType(s) == COMMENT) {
            continue;
        }

        /* Add Label to symbol table and check for duplicates */
        if (statementGetLabel(s)) {
            is_label = true;

            SymtabEntry entry;
            if (statementGetType(s) == DIRECTIVE) {
                entry = symtabEntryCreate(statementGetLabel(s), dc, false, line_num, SYMBOL_DATA);
            } else {  // INSTRUCTION
                entry = symtabEntryCreate(statementGetLabel(s), ic, false, line_num, SYMBOL_CODE);
            }

            SymtabEntry found_entry;
            if (listFind(symtab, entry, (void **) &found_entry) == LIST_SUCCESS) {
                success = false;
                printf("Error: duplicate label '%s' on line %d. Was previously defined on line %d\n",
                       statementGetLabel(s), line_num, symtabEntryGetLineNum(found_entry));
                symtabEntryDestroy(found_entry);
            } else {
                listAppend(symtab, entry);
            }
            symtabEntryDestroy(entry);
        } else {
            is_label = false;
        }

        if (statementGetType(s) == DIRECTIVE) {
            const char *directive = statementGetMnemonic(s);
            if (is_label && isDataStoreDirective(directive)) {
                dc += calcDirectiveDataSize(s);
            } else { // .extern or .entry
                if (strcmp(directive, DIRECTIVE_EXTERN) == 0) {
                    List extern_operands = statementGetOperands(s);
                    for (int i = 0; i < listLength(extern_operands); i++) {
                        const char *extern_operand = listGetDataAt(extern_operands, i);

                        SymtabEntry entry = symtabEntryCreate(extern_operand, 0, true, line_num, SYMBOL_EXTERN);

                        SymtabEntry found_entry;
                        if (listFind(symtab, entry, (void **) &found_entry) == LIST_SUCCESS) {
                            success = false;
                            printf("Error: duplicate extern label '%s' on line %d was previously defined on line %d\n",
                                   extern_operand, line_num, symtabEntryGetLineNum(found_entry));
                            symtabEntryDestroy(found_entry);
                        } else {
                            listAppend(symtab, entry);
                        }
                        symtabEntryDestroy(entry);
                    }
                }
            }
        } else { // INSTRUCTION

            ic++;
        }
    }

    if (!success) {
        listDestroy(symtab);
        return NULL;
    }
    return symtab;
}

/**
 * It runs the first pass of the assembler.
 *
 * @param filename The name of the file to be read.
 * @return The built symbol table.
 */
List run_first_pass(const char *filename) {
    FILE *src_file = openFileWithSuffix(filename, "r", SOURCE_FILE_SUFFIX);
    /* Building the symbol table. */
    List symtab = buildSymbolTable(src_file);
    fclose(src_file);

    return symtab;
}
