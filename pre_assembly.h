#ifndef ASSEMBLER_PRE_ASSEMBLY_H
#define ASSEMBLER_PRE_ASSEMBLY_H

#include <stdbool.h>

#define AFTER_MACRO_SUFFIX ".am"

bool run_pre_assembly(const char *filename);

#endif
