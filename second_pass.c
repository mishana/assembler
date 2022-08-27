//
// Created by misha on 27/07/2022.
//

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "second_pass.h"
#include "linkedlist.h"
#include "file_utils.h"
#include "machine_code.h"
#include "memory_code.h"
#include "const_tables.h"
#include "symtab.h"
#include "base_conversion.h"

#define SOURCE_FILE_SUFFIX ".am"
#define START_ADDRESS_OFFSET 100


/**
 * It writes the machine and memory codes to the object file.
 *
 * @param machine_codes a list of machine codes
 * @param memory_codes a list of memory codes
 * @param obj_file the file pointer to the object file
 */
static void writeCodeToObjectFile(List machine_codes, List memory_codes, FILE *obj_file) {
    size_t machine_code_size = 0, memory_code_size = 0;
    for (int i = 0; i < listLength(machine_codes); i++) {
        MachineCode mc = (MachineCode) listGetDataAt(machine_codes, i);
        machine_code_size += machineCodeGetSize(mc);
    }
    for (int i = 0; i < listLength(memory_codes); i++) {
        MemoryCode mc = (MemoryCode) listGetDataAt(memory_codes, i);
        memory_code_size += memoryCodeGetSize(mc);
    }
    char binary_buf1[BINARY_WORD_SIZE + 1];
    char binary_buf2[BINARY_WORD_SIZE + 1];
    char base32_buf1[BASE32_WORD_SIZE + 1];
    char base32_buf2[BASE32_WORD_SIZE + 1];

    decimalToBinary(machine_code_size, binary_buf1, BINARY_WORD_SIZE);
    binaryToBase32Word(binary_buf1, base32_buf1);

    decimalToBinary(memory_code_size, binary_buf2, BINARY_WORD_SIZE);
    binaryToBase32Word(binary_buf2, base32_buf2);

    fprintf(obj_file, "%s %s\n", base32_buf1, base32_buf2);

    for (int i = 0; i < listLength(machine_codes); i++) {
        MachineCode mc = (MachineCode) listGetDataAt(machine_codes, i);
        machineCodeToObjFile(mc, obj_file, START_ADDRESS_OFFSET);
    }
    for (int i = 0; i < listLength(memory_codes); i++) {
        MemoryCode mc = (MemoryCode) listGetDataAt(memory_codes, i);
        memoryCodeToObjFile(mc, obj_file, START_ADDRESS_OFFSET);
    }
}

/**
 * It updates the addresses of the symbols in the machine code from the symbol table.
 *
 * @param machine_codes a list of machine codes
 * @param symtab a list of symbols (see below)
 * @param filename the name of the file that contains the assembly code
 */
static bool updateAdressesFromSymtab(List machine_codes, List symtab, const char *filename) {
    bool success = true;
    for (int i = 0; i < listLength(machine_codes); ++i) {
        MachineCode mc = (MachineCode) listGetDataAt(machine_codes, i);
        success =
                success && machineCodeUpdateFromSymtab(mc, symtab, SOURCE_FILE_SUFFIX, filename, START_ADDRESS_OFFSET);
    }
    return success;
}

/**
 * It updates the symbol table with the the declared .entry symbols.
 *
 * @param filename the name of the file being processed
 * @param src_file The file pointer to the source file.
 * @param symtab a pointer to a List (which is a pointer to a struct)
 */
bool updateEntriesInSymbolTable(const char *filename, FILE *src_file, List symtab) {
    bool success = true;

    int line_num = 0;
    char line[LINE_BUFFER_LEN];
    while (fgets(line, LINE_BUFFER_LEN, src_file) != NULL) {
        line_num++;
        Statement s = parse(line, line_num, filename, SOURCE_FILE_SUFFIX);
        if (statementGetType(s) == EMPTY_LINE || statementGetType(s) == COMMENT) {
            continue;
        }
        if (statementGetType(s) == DIRECTIVE) {
            const char *directive = statementGetMnemonic(s);
            if (strcmp(directive, DIRECTIVE_ENTRY) == 0) {
                List entry_operands = statementGetOperands(s);
                assert(listLength(entry_operands) == 1);

                const char *entry_operand = listGetDataAt(entry_operands, 0);
                SymtabEntry found_entry = symbolTableFindByName(symtab, entry_operand);
                if (!found_entry) {
                    printf("Error in %s.%s line %d: entry '%s' not found\n", filename, SOURCE_FILE_SUFFIX,
                           line_num, entry_operand);
                    success = false;
                    continue;
                }
                if (symtabEntryGetType(found_entry) == SYMBOL_EXTERN) {
                    printf("Error in %s.%s line %d: can't define '%s' as both .extern and .entry\n", filename,
                           SOURCE_FILE_SUFFIX,
                           line_num, entry_operand);
                    success = false;
                    continue;
                }
                symtabEntrySetIsEntry(found_entry, true);
            }
        }
    }
    return success;
}

/**
 * It writes the declared .entry symbols to the .ent file.
 *
 * @param symtab a list of symbol table entries
 * @param filename the name of the file to write to
 */
void writeEntriesFile(List symtab, const char *filename) {
    bool is_entry = false;
    FILE *entries_file = openFileWithSuffix(filename, "w", ENTRIES_FILE_SUFFIX);
    for (int i = 0; i < listLength(symtab); ++i) {
        SymtabEntry entry = (SymtabEntry) listGetDataAt(symtab, i);
        if (symtabEntryIsEntry(entry)) {
            is_entry = true;
            char binary_buf[BINARY_WORD_SIZE + 1];
            decimalToBinary(symtabEntryGetValue(entry) + START_ADDRESS_OFFSET, binary_buf, BINARY_WORD_SIZE);
            char base32_buf[BASE32_WORD_SIZE + 1];
            binaryToBase32Word(binary_buf, base32_buf);
            fprintf(entries_file, "%s %s\n", symtabEntryGetName(entry), base32_buf);
        }
    }
    fclose(entries_file);

    if (!is_entry) {
        removeFileWithSuffix(filename, ENTRIES_FILE_SUFFIX);
    } else {
        printf("%s%s file created\n", filename, ENTRIES_FILE_SUFFIX);
    }
}

/**
 * It writes the external declarations and usages to the .ext file.
 *
 * @param machine_codes A list of machine code instructions.
 * @param filename the name of the file to write to
 */
void writeExternalFile(List machine_codes, const char *filename) {
    bool is_extern = false;
    FILE *extern_file = openFileWithSuffix(filename, "w", EXTERNAL_FILE_SUFFIX);
    for (int i = 0; i < listLength(machine_codes); ++i) {
        MachineCode mc = (MachineCode) listGetDataAt(machine_codes, i);
        for (int j = 0; j < machineCodeGetNumOperands(mc); ++j) {
            if (machineCodeGetIsExternOperand(mc, j)) {
                is_extern = true;
                char binary_buf[BINARY_WORD_SIZE + 1];
                decimalToBinary(machineCodeGetExternalOperandAddress(mc, j) + START_ADDRESS_OFFSET, binary_buf,
                                BINARY_WORD_SIZE);
                char base32_buf[BASE32_WORD_SIZE + 1];
                binaryToBase32Word(binary_buf, base32_buf);
                fprintf(extern_file, "%s %s\n", machineCodeGetOperand(mc, j), base32_buf);
            }
        }
    }
    fclose(extern_file);

    if (!is_extern) {
        removeFileWithSuffix(filename, EXTERNAL_FILE_SUFFIX);
    } else {
        printf("%s%s file created\n", filename, EXTERNAL_FILE_SUFFIX);
    }
}

/**
 * Runs the second pass of the assembler.
 *
 * @param filename the name of the file to be read
 * @param symtab a list of symbols and their addresses
 * @param machine_codes a list of machine codes
 * @param memory_codes a list of memory codes
 */
bool run_second_pass(const char *filename, List symtab, List machine_codes, List memory_codes) {
    FILE *src_file = openFileWithSuffix(filename, "r", SOURCE_FILE_SUFFIX);
    FILE *object_file = openFileWithSuffix(filename, "w", OBJECT_FILE_SUFFIX);

    bool success = updateAdressesFromSymtab(machine_codes, symtab, filename);
    writeCodeToObjectFile(machine_codes, memory_codes, object_file);
    success = success && updateEntriesInSymbolTable(filename, src_file, symtab);
    writeEntriesFile(symtab, filename);
    writeExternalFile(machine_codes, filename);

    fclose(src_file);
    fclose(object_file);

    listDestroy(symtab);
    listDestroy(machine_codes);
    listDestroy(memory_codes);

    return success;
}
