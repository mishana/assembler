//
// Created by misha on 27/07/2022.
//
#include <string.h>

#include "symtab.h"


const char *DIRECTIVES[] = {".data", ".string", ".struct", ".entry", ".extern"};
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
 * It checks if the string is a directive.
 *
 * @param str The string to check.
 */
bool isDirective(const char *str) {
    for (int i = 0; i < DIRECTIVES_SIZE; ++i) {
        if (strcmp(str, DIRECTIVES[i]) == 0)
            return true;
    }
    return false;
}

/**
 * It checks if the string is an instruction.
 *
 * @param str The string to check
 */
bool isInstruction(const char *str) {
    for (int i = 0; i < INSTRUCTIONS_ALL_SIZE; ++i) {
        if (strcmp(str, INSTRUCTIONS_ALL[i]) == 0)
            return true;
    }
    return false;
}
