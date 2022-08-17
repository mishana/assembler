//
// Created by misha on 15/08/2022.
//

#ifndef ASSEMBLER_RULES_H
#define ASSEMBLER_RULES_H

#include <stdbool.h>

bool isValidAddressing_2_OP(const char *instruction, int src_addressing_code, int dst_addressing_code);

bool isValidAddressing_1_OP(const char *instruction, int addressing_code);

#endif //ASSEMBLER_RULES_H
