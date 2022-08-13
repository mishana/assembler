//
// Created by misha on 27/07/2022.
//

#ifndef ASSEMBLER_SYMTAB_H
#define ASSEMBLER_SYMTAB_H

#include <stdbool.h>

#define DIRECTIVES_SIZE 5
#define INSTRUCTIONS_2_OP_SIZE 5
#define INSTRUCTIONS_1_OP_SIZE 9
#define INSTRUCTIONS_0_OP_SIZE 2
#define INSTRUCTIONS_ALL_SIZE INSTRUCTIONS_2_OP_SIZE + INSTRUCTIONS_1_OP_SIZE + INSTRUCTIONS_0_OP_SIZE

extern const char *DIRECTIVES[];
extern const char *INSTRUCTIONS_2_OP[];
extern const char *INSTRUCTIONS_1_OP[];
extern const char *INSTRUCTIONS_0_OP[];
extern const char *INSTRUCTIONS_ALL[];
extern const char *REGISTERS[];

bool isDirective(const char *str);

bool isInstruction(const char *str);


#endif //ASSEMBLER_SYMTAB_H
