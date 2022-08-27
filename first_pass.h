#ifndef ASSEMBLER_FIRST_PASS_H
#define ASSEMBLER_FIRST_PASS_H

#include "linkedlist.h"

bool run_first_pass(const char *filename, List *symtab_ptr, List *machine_codes_ptr, List *memory_codes_ptr);

#endif
