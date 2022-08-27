#include <string.h>
#include <ctype.h>
#include "const_tables.h"
#include "str_utils.h"
#include "types_utils.h"

#define NOT_FOUND -1

#define IMMEDIATE_ADDRESSING_PREFIX '#'
#define REGISTER_PREFIX 'r'


const char *DIRECTIVES[] = {DIRECTIVE_DATA, DIRECTIVE_STRING, DIRECTIVE_STRUCT, DIRECTIVE_ENTRY, DIRECTIVE_EXTERN};
const char *INSTRUCTIONS_2_OP[] = {"mov", "cmp", "add", "sub", "lea"};
const char *INSTRUCTIONS_1_OP[] = {"not", "clr", "inc", "dec", "jmp", "bne", "get", "prn",
                                   "jsr"};
const char *INSTRUCTIONS_0_OP[] = {"rts", "hlt"};
const char *INSTRUCTIONS_ALL[] = {"mov", "cmp", "add", "sub",
                                  "not", "clr", "lea", "inc",
                                  "dec", "jmp", "bne", "get",
                                  "prn", "jsr", "rts", "hlt"};
const char *REGISTERS[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};


/**
 * It checks if the word is a reserved word.
 *
 * @param word The word to check
 */
bool isReservedWord(const char *word) {
    int i;
    for (i = 0; i < DIRECTIVES_SIZE; i++) {
        if (strcmp(word, DIRECTIVES[i] + 1) == 0) { /* +1 to skip the '.' */
            return true;
        }
    }
    for (i = 0; i < INSTRUCTIONS_ALL_SIZE; i++) {
        if (strcmp(word, INSTRUCTIONS_ALL[i]) == 0) {
            return true;
        }
    }
    for (i = 0; i < REGISTERS_SIZE; i++) {
        if (strcmp(word, REGISTERS[i]) == 0) {
            return true;
        }
    }
    return false;
}

/**
 * It returns the instruction code for the given instruction.
 *
 * @param instruction The instruction to get the code for.
 */
int getInstructionCode(const char *instruction) {
    int i;
    for (i = 0; i < INSTRUCTIONS_ALL_SIZE; i++) {
        if (strcmp(instruction, INSTRUCTIONS_ALL[i]) == 0) {
            return i;
        }
    }
    return NOT_FOUND;
}

/**
 * It checks if the instruction is a 0 operand instruction.
 *
 * @param instruction The instruction to check
 */
bool is0OperandInstruction(const char *instruction) {
    int i;
    for (i = 0; i < INSTRUCTIONS_0_OP_SIZE; ++i) {
        if (strcmp(instruction, INSTRUCTIONS_0_OP[i]) == 0)
            return true;
    }
    return false;
}

/**
 * It checks if the instruction is a 1 operand instruction.
 *
 * @param instruction The instruction to check
 */
bool is1OperandInstruction(const char *instruction) {
    int i;
    for (i = 0; i < INSTRUCTIONS_1_OP_SIZE; ++i) {
        if (strcmp(instruction, INSTRUCTIONS_1_OP[i]) == 0)
            return true;
    }
    return false;
}

/**
 * It checks if the instruction is a 2 operand instruction.
 *
 * @param instruction The instruction to check
 */
bool is2OperandInstruction(const char *instruction) {
    int i;
    for (i = 0; i < INSTRUCTIONS_2_OP_SIZE; ++i) {
        if (strcmp(instruction, INSTRUCTIONS_2_OP[i]) == 0)
            return true;
    }
    return false;
}

/**
 * It returns the number of operands for a given instruction.
 *
 * @param instruction The instruction to get the number of operands for.
 */
int getInstructionNumberOfOperands(const char *instruction) {
    if (is0OperandInstruction(instruction)) {
        return 0;
    } else if (is1OperandInstruction(instruction)) {
        return 1;
    } else if (is2OperandInstruction(instruction)) {
        return 2;
    }
    return -1;
}

/**
 * It returns the addressing mode of the operand.
 *
 * @param operand The operand string.
 */
AddressingMode getAddressingMode(const char *operand) {
    AddressingMode res;

    if (operand[0] == IMMEDIATE_ADDRESSING_PREFIX) {
        if (isNumeric(operand + 1)) {
            return IMMEDIATE_ADDRESSING;
        } else {
            return INVALID_ADDRESSING;
        }
    } else if (operand[0] == REGISTER_PREFIX && strlen(operand) == 2 && operand[1] >= '0' && operand[1] <= '7') {
        return REGISTER_ADDRESSING;
    } else if (isdigit(operand[0])) {
        return INVALID_ADDRESSING;
    } else if (isAlphaNumeric(operand)) {
        return DIRECT_ADDRESSING;
    } else {
        List split_operand = strSplit(operand, ".");
        const char *before_delim = listGetDataAt(split_operand, 0);
        const char *after_delim = listGetDataAt(split_operand, 1);

        if (listLength(split_operand) == 2 && (strcmp(after_delim, "1") == 0 ||
                                               strcmp(after_delim, "2") == 0) &&
            strlen(before_delim) > 1 && isAlphaNumeric(before_delim)) {
            res = STRUCT_ADDRESSING;
        } else {
            res = INVALID_ADDRESSING;
        }
        listDestroy(split_operand);
        return res;
    }
}
