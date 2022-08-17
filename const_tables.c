//
// Created by misha on 27/07/2022.
//

#include <string.h>
#include "const_tables.h"

#define NOT_FOUND -1


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
