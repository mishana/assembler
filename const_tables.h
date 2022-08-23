//
// Created by misha on 27/07/2022.
//

#ifndef ASSEMBLER_CONST_TABLES_H
#define ASSEMBLER_CONST_TABLES_H

#include <stdbool.h>
#include "linkedlist.h"

#define DIRECTIVES_SIZE 5
#define INSTRUCTIONS_2_OP_SIZE 5
#define INSTRUCTIONS_1_OP_SIZE 9
#define INSTRUCTIONS_0_OP_SIZE 2
#define INSTRUCTIONS_ALL_SIZE INSTRUCTIONS_2_OP_SIZE + INSTRUCTIONS_1_OP_SIZE + INSTRUCTIONS_0_OP_SIZE

#define A 0
#define E 1
#define R 2

#define DIRECTIVE_DATA ".data"
#define DIRECTIVE_STRING ".string"
#define DIRECTIVE_STRUCT ".struct"
#define DIRECTIVE_ENTRY ".entry"
#define DIRECTIVE_EXTERN ".extern"

typedef enum {
    IMMEDIATE_ADDRESSING,
    DIRECT_ADDRESSING,
    STRUCT_ADDRESSING,
    REGISTER_ADDRESSING,
    INVALID_ADDRESSING
} AddressingMode;

extern const char *DIRECTIVES[];
extern const char *INSTRUCTIONS_2_OP[];
extern const char *INSTRUCTIONS_1_OP[];
extern const char *INSTRUCTIONS_0_OP[];
extern const char *INSTRUCTIONS_ALL[];
extern const char *REGISTERS[];


int getDirectiveCode(const char *directive);

int getInstructionCode(const char *instruction);

int getInstructionNumberOfOperands(const char *instruction);

AddressingMode get_addressing_mode(const char *operand, List symbol_table);

#endif //ASSEMBLER_CONST_TABLES_H
