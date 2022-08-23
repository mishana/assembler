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
#include "machine_code.h"

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
 * @param filename The name of the source file.
 */
List buildSymbolTable(FILE *src_file, const char *filename, List machine_codes) {
    List symtab = listCreate((list_eq) symtabEntryCmp, (list_copy) symtabEntryCopy, (list_free) symtabEntryDestroy);

    size_t ic = 0, dc = 0;
    bool is_label = false;

    bool success = true;

    int line_num = 0;
    char line[MAX_LINE_LEN];
    while (fgets(line, MAX_LINE_LEN, src_file) != NULL) {
        line_num++;
        Statement s = parse(line, line_num);
        if (!s || !statementCheckSyntax(s, filename, SOURCE_FILE_SUFFIX)) {
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
                const char *directive = statementGetMnemonic(s);
                bool is_struct = strcmp(directive, DIRECTIVE_STRUCT) == 0;
                entry = symtabEntryCreate(statementGetLabel(s), dc, false, is_struct, line_num, SYMBOL_DATA);
            } else {  // INSTRUCTION
                entry = symtabEntryCreate(statementGetLabel(s), ic, false, false, line_num, SYMBOL_CODE);
            }

            SymtabEntry found_entry;
            if (listFind(symtab, entry, (void **) &found_entry) == LIST_SUCCESS) {
                success = false;
                printf("Error in %s.%s line %d: duplicate label '%s' was previously defined on line %d\n",
                       filename, SOURCE_FILE_SUFFIX, line_num, statementGetLabel(s), symtabEntryGetLineNum(found_entry));
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
                // TODO: map the data in memory
                dc += calcDirectiveDataSize(s);
            } else { // .extern or .entry
                if (strcmp(directive, DIRECTIVE_EXTERN) == 0) {
                    List extern_operands = statementGetOperands(s);
                    for (int i = 0; i < listLength(extern_operands); i++) {
                        const char *extern_operand = listGetDataAt(extern_operands, i);

                        SymtabEntry entry = symtabEntryCreate(extern_operand, 0, true, false, line_num, SYMBOL_EXTERN);

                        SymtabEntry found_entry;
                        if (listFind(symtab, entry, (void **) &found_entry) == LIST_SUCCESS) {
                            success = false;
                            printf("Error in %s.%s line %d: duplicate extern label '%s' was previously defined on line %d\n",
                                   filename, SOURCE_FILE_SUFFIX, line_num, extern_operand, symtabEntryGetLineNum(found_entry));
                            symtabEntryDestroy(found_entry);
                        } else {
                            listAppend(symtab, entry);
                        }
                        symtabEntryDestroy(entry);
                    }
                }
            }
        } else { // INSTRUCTION
            MachineCode mc = machineCodeCreate(s, ic);
            listAppend(machine_codes, mc);
            ic += machineCodeGetSize(mc);
            machineCodeDestroy(mc);
        }
    }

    if (!success) {
        listDestroy(symtab);
        return NULL;
    }

    /* Adding the IC to the data symbols addresses. */
    for (int i = 0; i < listLength(symtab); i++) {
        SymtabEntry entry = (SymtabEntry)listGetDataAt(symtab, i);
        if (symtabEntryGetType(entry) == SYMBOL_DATA) {
            symtabEntrySetValue(entry, symtabEntryGetValue(entry) + ic);
        }
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
    List machine_codes = listCreate((list_eq) machineCodeCmp, (list_copy) machineCodeCopy, (list_free) machineCodeDestroy);
    List symtab = buildSymbolTable(src_file, filename, machine_codes);
    fclose(src_file);

    return symtab;
}
