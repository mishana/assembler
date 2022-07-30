//
// Created by misha on 27/07/2022.
//
#include <string.h>

#include "symtab.h"

#define ALL_MNEMONICS_SIZE 5 + 5 + 9 + 2

const char *ALL_MNEMONICS[ALL_MNEMONICS_SIZE] = {
        ".data", ".string", ".struct", ".entry", ".extern",
        "mov", "cmp", "add", "sub", "lea",
        "not", "clr", "inc", "dec", "jmp", "bne", "get", "prn", "jsr",
        "rts", "hlt"
};

bool isDirectiveOrInstruction(const char *str) {
    for (int i = 0; i < ALL_MNEMONICS_SIZE; ++i) {
        if (strcmp(str, ALL_MNEMONICS[i]) == 0)
            return true;
    }
    return false;
}