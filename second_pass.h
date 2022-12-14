//
// Created by misha on 27/07/2022.
//

#ifndef ASSEMBLER_SECOND_PASS_H
#define ASSEMBLER_SECOND_PASS_H

#include "linkedlist.h"

#define OBJECT_FILE_SUFFIX ".ob"
#define ENTRIES_FILE_SUFFIX ".ent"
#define EXTERNAL_FILE_SUFFIX ".ext"

bool run_second_pass(const char *filename, List symtab, List machine_codes, List memory_codes);

#endif //ASSEMBLER_SECOND_PASS_H
