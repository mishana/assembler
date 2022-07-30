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

bool isDirectiveOrInstruction(const char *str);


#endif //ASSEMBLER_SYMTAB_H
