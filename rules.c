//
// Created by misha on 15/08/2022.
//

#include <string.h>
#include <assert.h>
#include "rules.h"
#include "const_tables.h"


/**
 * It checks if the addressing modes of the source and destination operands are valid for the given 2-op instruction
 *
 * @param instruction_code the code of the instruction (e.g. MOV, CMP, ADD, etc.)
 * @param src_addressing_code the addressing code of the source operand
 * @param dst_addressing_code the addressing code of the destination operand
 */
bool isValidAddressing_2_OP(int instruction_code, int src_addressing_code, int dst_addressing_code) { // TODO: maybe change instruction_code to const char *instruction
    assert(instruction_code >= 0 && instruction_code < INSTRUCTIONS_ALL_SIZE);

    const char *instruction = INSTRUCTIONS_ALL[instruction_code];

    if (strcmp(instruction, "mov") == 0 || strcmp(instruction, "add") == 0 || strcmp(instruction, "sub") == 0) {
        return dst_addressing_code != IMMEDIATE_ADDRESSING;
    } else if (strcmp(instruction, "cmp") == 0) {
        return true;
    } else if (strcmp(instruction, "lea") == 0) {
        return (src_addressing_code == DIRECT_ADDRESSING || src_addressing_code == STRUCT_ADDRESSING)
               && dst_addressing_code != IMMEDIATE_ADDRESSING;
    } else { // not a valid 2-op instruction
        return false;
    }
}

/**
 * It checks if the addressing mode of the destination operand is valid for the given 1-op instruction
 *
 * @param instruction_code the code of the instruction (e.g. MOV, ADD, etc.)
 * @param addressing_code the addressing code of the instruction
 */
bool isValidAddressing_1_OP(int instruction_code, int addressing_code) {  // TODO: maybe change instruction_code to const char *instruction
    assert(instruction_code >= 0 && instruction_code < INSTRUCTIONS_ALL_SIZE);

    const char *instruction = INSTRUCTIONS_ALL[instruction_code];

    if (strcmp(instruction, "not") == 0 || strcmp(instruction, "clr") == 0 || strcmp(instruction, "inc") == 0 ||
        strcmp(instruction, "dec") == 0 || strcmp(instruction, "jmp") == 0 || strcmp(instruction, "bne") == 0 ||
        strcmp(instruction, "get") == 0 || strcmp(instruction, "jsr") == 0) {
        return addressing_code != IMMEDIATE_ADDRESSING;
    } else if (strcmp(instruction, "prn") == 0) {
        return true;
    } else { // not a valid 1-op instruction
        return false;
    }
}
