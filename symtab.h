//
// Created by misha on 27/07/2022.
//

#ifndef ASSEMBLER_SYMTAB_H
#define ASSEMBLER_SYMTAB_H

#include <stdbool.h>

const char *DIRECTIVES[] = {".data", ".string", ".struct", ".entry", ".extern"};
const char *INSTRUCTIONS_2_OP[] = {"mov", "cmp", "add", "sub", "lea"};
const char *INSTRUCTIONS_1_OP[] = {"not", "clr", "inc", "dec", "jmp", "bne", "get", "prn",
                                  "jsr"};
const char *INSTRUCTIONS_0_OP[] = {"rts", "hlt"};
const char *REGISTERS[] = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7"};

bool isDirectiveOrInstruction(const char *str);


#endif //ASSEMBLER_SYMTAB_H
