//
// Created by misha on 27/07/2022.
//

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include "utils.h"
#include "errors.h"

// Constants and MACROs
#define BASE32 32
#define BASE32_WORD_SIZE 2
#define BAD_BASE32_DIGIT -1

char BASE32_DIGITS[] = {'!', '@', '#', '$', '%', '^', '&', '*', '<', '>',
                        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                        'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
                        'r', 's', 't', 'u', 'v'};


int base32DigitToDecimal(char base32_digit) {
    for (int i = 0; i < BASE32; ++i) {
        if (BASE32_DIGITS[i] == base32_digit)
            return i;
    }
    return BAD_BASE32_DIGIT;
}

int base32WordToDecimal(const char *base32_word) {
    // We assume a 2-digit base32 word
    assert(strlen(base32_word) == BASE32_WORD_SIZE);

    return base32DigitToDecimal(base32_word[0]) * BASE32 +
           base32DigitToDecimal(base32_word[1]);
}

const char *decimalToBase32Word(int decimal) {
    // We assume a 2-digit base32 word
    assert(decimal < (int)pow(BASE32, BASE32_WORD_SIZE));

    char* base32_word = (char*)malloc(sizeof(char) * BASE32_WORD_SIZE);
    if (base32_word == NULL)
        exit(MEMORY_ALLOCATION_ERROR);

    base32_word[0] = BASE32_DIGITS[(int)(decimal / BASE32)];
    base32_word[1] = BASE32_DIGITS[(int)(decimal % BASE32)];

    return base32_word;
}

int binaryToDecimal(const char* binary) {
    // TODO
}

const char* decimalToBinary(int decimal) {
    int binary_len = (int)ceil(log2(decimal));
    char* binary = (char*)malloc(sizeof(char) * binary_len);
    // TODO
}

