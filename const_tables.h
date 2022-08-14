//
// Created by misha on 27/07/2022.
//

#ifndef ASSEMBLER_CONST_TABLES_H
#define ASSEMBLER_CONST_TABLES_H

#include <stdbool.h>

#define DIRECTIVES_SIZE 5
#define INSTRUCTIONS_2_OP_SIZE 5
#define INSTRUCTIONS_1_OP_SIZE 9
#define INSTRUCTIONS_0_OP_SIZE 2
#define INSTRUCTIONS_ALL_SIZE INSTRUCTIONS_2_OP_SIZE + INSTRUCTIONS_1_OP_SIZE + INSTRUCTIONS_0_OP_SIZE

#define A 0
#define E 1
#define R 2

#define IMMEDIATE_ADDRESSING 0
#define DIRECT_ADDRESSING 1
#define STRUCT_ADDRESSING 2
#define REGISTER_ADDRESSING 3

extern const char *DIRECTIVES[];
extern const char *INSTRUCTIONS_2_OP[];
extern const char *INSTRUCTIONS_1_OP[];
extern const char *INSTRUCTIONS_0_OP[];
extern const char *INSTRUCTIONS_ALL[];
extern const char *REGISTERS[];


int getDirectiveCode(const char *directive);

int getInstructionCode(const char *instruction);

#endif //ASSEMBLER_CONST_TABLES_H
