//
// Created by misha on 15/08/2022.
//

#include <string.h>
#include "rules.h"
#include "const_tables.h"


/**
 * It checks if the addressing modes of the source and destination operands are valid for the given 2-op instruction
 *
 * @param instruction_code the code of the instruction (e.g. MOV, CMP, ADD, etc.)
 * @param src_addressing_mode the addressing code of the source operand
 * @param dst_addressing_mode the addressing code of the destination operand
 */
bool isValidAddressing_2_OP(const char *instruction, int src_addressing_mode, int dst_addressing_mode) {
    if (strcmp(instruction, "mov") == 0 || strcmp(instruction, "add") == 0 || strcmp(instruction, "sub") == 0) {
        return dst_addressing_mode != IMMEDIATE_ADDRESSING;
    } else if (strcmp(instruction, "cmp") == 0) {
        return true;
    } else if (strcmp(instruction, "lea") == 0) {
        return (src_addressing_mode == DIRECT_ADDRESSING || src_addressing_mode == STRUCT_ADDRESSING)
               && dst_addressing_mode != IMMEDIATE_ADDRESSING;
    } else { // not a valid 2-op instruction
        return false;
    }
}

/**
 * It checks if the addressing mode of the destination operand is valid for the given 1-op instruction
 *
 * @param instruction_code the code of the instruction (e.g. MOV, ADD, etc.)
 * @param addressing_mode the addressing code of the instruction
 */
bool isValidAddressing_1_OP(const char *instruction, int addressing_mode) {
    if (strcmp(instruction, "not") == 0 || strcmp(instruction, "clr") == 0 || strcmp(instruction, "inc") == 0 ||
        strcmp(instruction, "dec") == 0 || strcmp(instruction, "jmp") == 0 || strcmp(instruction, "bne") == 0 ||
        strcmp(instruction, "get") == 0 || strcmp(instruction, "jsr") == 0) {
        return addressing_mode != IMMEDIATE_ADDRESSING;
    } else if (strcmp(instruction, "prn") == 0) {
        return true;
    } else { // not a valid 1-op instruction
        return false;
    }
}
