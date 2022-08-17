//
// Created by misha on 27/07/2022.
//

#include <string.h>
#include "const_tables.h"
#include "str_utils.h"

#define NOT_FOUND -1

#define IMMEDIATE_ADDRESSING_PREFIX '#'


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


int getDirectiveCode(const char *directive) {
    for (int i = 0; i < DIRECTIVES_SIZE; i++) {
        if (strcmp(directive, DIRECTIVES[i]) == 0) {
            return i;
        }
    }
    return NOT_FOUND;
}

int getInstructionCode(const char *instruction) {
    for (int i = 0; i < INSTRUCTIONS_ALL_SIZE; i++) {
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
    for (int i = 0; i < INSTRUCTIONS_0_OP_SIZE; ++i) {
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
    for (int i = 0; i < INSTRUCTIONS_1_OP_SIZE; ++i) {
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
    for (int i = 0; i < INSTRUCTIONS_2_OP_SIZE; ++i) {
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

int get_addressing_mode(const char *operand) {
    // TODO: continue here, also need to think how to get the values of numericall operands. (e.g. #-10)
    if (operand[0] == IMMEDIATE_ADDRESSING_PREFIX) {
        return IMMEDIATE_ADDRESSING;
    } else if (strStartsWith(operand, REGISTER_PREFIX)) {
        return REGISTER_ADDRESSING;
    } else if (strStartsWith(operand, LABEL_PREFIX)) {
        return STRUCT_ADDRESSING;
    } else if (){
        return DIRECT_ADDRESSING;
    }
}

