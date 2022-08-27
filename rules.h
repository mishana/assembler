#ifndef ASSEMBLER_RULES_H
#define ASSEMBLER_RULES_H

#include <stdbool.h>
#include "linkedlist.h"

bool isValidAddressing_2_OP(const char *instruction, int src_addressing_mode, int dst_addressing_mode);

bool isValidAddressing_1_OP(const char *instruction, int addressing_mode);

#endif
