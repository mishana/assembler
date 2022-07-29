//
// Created by misha on 27/07/2022.
//

#ifndef ASSEMBLER_BASE_CONVERSION_H
#define ASSEMBLER_BASE_CONVERSION_H

int base32WordToDecimal(const char *base32_word);
void decimalToBase32Word(int decimal, char* base32_word);
int binaryToDecimal(const char* binary);
void decimalToBinary(int decimal, char* binary);

#endif //ASSEMBLER_BASE_CONVERSION_H
