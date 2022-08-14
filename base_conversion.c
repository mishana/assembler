//
// Created by misha on 27/07/2022.
//

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "base_conversion.h"
#include "errors.h"

// Constants and MACROs
#define BASE32 32


char BASE32_DIGITS[] = {'!', '@', '#', '$', '%', '^', '&', '*', '<', '>',
                        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                        'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
                        'r', 's', 't', 'u', 'v'};


/**
 * It converts a base32 digit to a decimal number.
 *
 * @param base32_digit The base32 digit to convert to decimal.
 */
int base32DigitToDecimal(char base32_digit) {
    for (int i = 0; i < BASE32; ++i) {
        if (BASE32_DIGITS[i] == base32_digit)
            return i;
    }

    /* A function that prints an error message and exits the program. */
    errorWithMsg("Bad Base32 Digit ERROR!");
    return -1; // just to avoid compiler warnings
}

/**
 * Convert a base32 word to a decimal number.
 *
 * @param base32_word The base32 word to convert to decimal.
 */
int base32WordToDecimal(const char *base32_word) {
    // We assume a 2-digit base32 word
    assert(strlen(base32_word) == BASE32_WORD_SIZE);

    return base32DigitToDecimal(base32_word[0]) * BASE32 +
           base32DigitToDecimal(base32_word[1]);
}

/**
 * It converts a decimal number to a base32 word.
 *
 * @param decimal the decimal number to convert
 * @param base32_word The base32 word that will be returned.
 */
void decimalToBase32Word(int decimal, char *base32_word) {
    // We assume a 2-digit base32 word
    assert(decimal < (int) pow(BASE32, BASE32_WORD_SIZE));

//    char* base32_word = (char*)malloc(sizeof(char) * (BASE32_WORD_SIZE + 1));
//    if (base32_word == NULL)
//        memoryAllocationError();

    base32_word[0] = BASE32_DIGITS[(int) (decimal / BASE32)];
    base32_word[1] = BASE32_DIGITS[(int) (decimal % BASE32)];
    base32_word[2] = '\0';
}

/**
 * Converts a binary string to a decimal integer.
 *
 * @param binary A string of 1's and 0's
 */
int binaryToDecimal(const char *binary) {
    size_t binary_len = strlen(binary);
    int decimal = 0;

    /* Calculating the weight of the most significant bit. */
    int bit_weight = (int) pow(2, binary_len - 1);
    for (size_t i = 0; i < binary_len; ++i) {
        assert(binary[i] == '0' || binary[i] == '1');

        /* It converts the character '0' or '1' to the integer 0 or 1. */
        int bit_value = binary[i] - '0';

        decimal += bit_value * bit_weight;
        bit_weight /= 2;
    }

    return decimal;
}

/**
 * It converts a decimal number to a binary string of 1's and 0's.
 *
 * @param decimal The decimal number to convert to binary.
 * @param binary The binary string that will be returned.
 */
void decimalToBinary(int decimal, char *binary, int min_num_bits) {
    int num_bits = (int) ceil(log2(decimal));
    num_bits = min_num_bits > num_bits ? min_num_bits : num_bits;
    /* Checking that the number of bits is less than or equal to 10. */
    assert(num_bits <= BINARY_WORD_SIZE);
//    char* binary = (char*)malloc(sizeof(char) * (num_bits + 1));
//
//    if (binary == NULL)
//        memoryAllocationError();

    /* Calculating the weight of the most significant bit. */
    int bit_weight = (int) pow(2, num_bits - 1);
    for (int i = 0; i < num_bits; ++i) {
        /* It checks if the bit is 1 or 0. */
        if (decimal / bit_weight >= 1) {
            binary[i] = '1';
            decimal -= bit_weight;
        } else {
            binary[i] = '0';
        }

        bit_weight /= 2;
    }
    /* Adding a null character to indicate the end of the actual binary string. */
    binary[num_bits] = '\0';
}

