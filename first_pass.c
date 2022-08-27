//
// Created by misha on 27/07/2022.
//

#include "first_pass.h"
#include "parser.h"
#include "str_utils.h"
#include "symtab.h"
#include "const_tables.h"
#include "file_utils.h"
#include "memory_code.h"
#include "machine_code.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

#define SOURCE_FILE_SUFFIX ".am"


/**
 * The function builds the symbol table.
 *
 * @param src_file The source file.
 * @param filename The name of the source file.
 */
bool run_first_pass_aux(FILE *src_file, const char *filename, List symtab, List machine_codes, List memory_codes) {
    size_t ic = 0, dc = 0;
    bool is_label = false;

    bool success = true;

    int line_num = 0;
    char line[LINE_BUFFER_LEN];
    while (fgets(line, LINE_BUFFER_LEN, src_file) != NULL) {
        if (strlen(line) > MAX_LINE_LEN) {
            success = false;
            printf("Error in %s.%s line %d: line too long, exceeds 80 characters\n",
                   filename, SOURCE_FILE_SUFFIX, line_num);
        }
        line_num++;
        Statement s = parse(line, line_num, filename, SOURCE_FILE_SUFFIX);
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
                       filename, SOURCE_FILE_SUFFIX, line_num, statementGetLabel(s),
                       symtabEntryGetLineNum(found_entry));
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
                MemoryCode mem_c = memoryCodeCreate(s, dc);

                listAppend(memory_codes, mem_c);
                dc += memoryCodeGetSize(mem_c);

                memoryCodeDestroy(mem_c);

            } else { // .extern or .entry
                if (strcmp(directive, DIRECTIVE_EXTERN) == 0) {
                    List extern_operands = statementGetOperands(s);
                    assert(listLength(extern_operands) == 1);

                    const char *extern_operand = listGetDataAt(extern_operands, 0);
                    SymtabEntry entry = symtabEntryCreate(extern_operand, 0, false, false, line_num, SYMBOL_EXTERN);

                    SymtabEntry found_entry;
                    if (listFind(symtab, entry, (void **) &found_entry) == LIST_SUCCESS) {
                        success = false;
                        printf("Error in %s.%s line %d: duplicate extern label '%s' was previously defined on line %d\n",
                               filename, SOURCE_FILE_SUFFIX, line_num, extern_operand,
                               symtabEntryGetLineNum(found_entry));
                    } else {
                        listAppend(symtab, entry);
                    }
                    symtabEntryDestroy(entry);
                }
            }
        } else { // INSTRUCTION
            MachineCode mc = machineCodeCreate(s, ic);

            listAppend(machine_codes, mc);
            ic += machineCodeGetSize(mc);

            machineCodeDestroy(mc);
        }
    }

    /* Adding the IC to the data symbols addresses. */
    for (int i = 0; i < listLength(symtab); i++) {
        SymtabEntry entry = (SymtabEntry) listGetDataAt(symtab, i);
        if (symtabEntryGetType(entry) == SYMBOL_DATA) {
            symtabEntrySetValue(entry, symtabEntryGetValue(entry) + ic);
        }
    }
    for (int i = 0; i < listLength(memory_codes); i++) {
        MemoryCode mem_c = (MemoryCode) listGetDataAt(memory_codes, i);
        memoryCodeSetStartAddress(mem_c, memoryCodeGetStartAddress(mem_c) + ic);
    }
    return success;
}

/**
 * It runs the first pass of the assembler.
 *
 * @param filename The name of the file to be read.
 * @return The built symbol table.
 */
bool run_first_pass(const char *filename, List *symtab_ptr, List *machine_codes_ptr, List *memory_codes_ptr) {
    FILE *src_file = openFileWithSuffix(filename, "r", SOURCE_FILE_SUFFIX);

    /* Building the symbol table and machine/memory codes. */
    *symtab_ptr = listCreate((list_eq) symtabEntryCmp, (list_copy) symtabEntryCopy, (list_free) symtabEntryDestroy);
    *machine_codes_ptr = listCreate((list_eq) machineCodeCmp, (list_copy) machineCodeCopy,
                                    (list_free) machineCodeDestroy);
    *memory_codes_ptr = listCreate((list_eq) memoryCodeCmp, (list_copy) memoryCodeCopy, (list_free) memoryCodeDestroy);

    bool res = run_first_pass_aux(src_file, filename, *symtab_ptr, *machine_codes_ptr, *memory_codes_ptr);

    fclose(src_file);

    return res;
}
