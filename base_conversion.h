#ifndef ASSEMBLER_BASE_CONVERSION_H
#define ASSEMBLER_BASE_CONVERSION_H

#define BASE32_WORD_SIZE 2
#define BINARY_WORD_SIZE 10

int binaryToDecimal(const char *binary, int num_bits);

char *decimalToBinary(int p_val, char *buf, size_t num_bits);

char *binaryToBase32Word(const char *binary, char *base32_word);

#endif
