//
// Created by misha on 27/07/2022.
//

#ifndef ASSEMBLER_BASE_CONVERSION_H
#define ASSEMBLER_BASE_CONVERSION_H

#define BASE32_WORD_SIZE 2
#define BINARY_WORD_SIZE 10

int base32WordToDecimal(const char *base32_word);

void decimalToBase32Word(int decimal, char *base32_word);

int binaryToDecimal(const char *binary);

void decimalToBinary(int decimal, char *binary, int min_num_bits);

#endif //ASSEMBLER_BASE_CONVERSION_H
