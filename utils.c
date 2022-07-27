//
// Created by misha on 27/07/2022.
//

#include "utils.h"

// Constants and MACRO's
#define BASE32 32

char BASE32_DIGITS[] = {'!', '@', '#', '$', '%', '^', '&', '*', '<', '>',
                        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                        'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
                        'r', 's', 't', 'u', 'v'};


int base32_word_to_decimal(const char* base32_str) {
    // We assume a 2-digit base32 word
    // TODO
}

const char* decimal_to_base32_word(int decimal) {
    // TODO
}

