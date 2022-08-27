#include <assert.h>
#include <string.h>
#include <math.h>
#include "base_conversion.h"

#define CHK(_n) ((_n) <= sz)


char BASE32_DIGITS[] = {'!', '@', '#', '$', '%', '^', '&', '*', '<', '>',
                        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
                        'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q',
                        'r', 's', 't', 'u', 'v'};


/**
 * Converts a binary string to a decimal integer.
 *
 * @param binary A string of 1's and 0's
 */
int binaryToDecimal(const char *binary, int num_bits) {
    int decimal, bit_weight, bit_value;
    size_t i;

    assert(num_bits <= strlen(binary));
    decimal = 0;

    /* Calculating the weight of the most significant bit. */
    bit_weight = (int) pow(2, num_bits - 1);
    for (i = 0; i < num_bits; ++i) {
        assert(binary[i] == '0' || binary[i] == '1');

        /* It converts the character '0' or '1' to the integer 0 or 1. */
        bit_value = binary[i] - '0';

        decimal += bit_value * bit_weight;
        bit_weight /= 2;
    }

    return decimal;
}

/**
 * Convert a decimal number to a binary string.
 *
 * @param p_val The decimal value to convert to binary.
 * @param buf The buffer to store the binary string in.
 * @param sz The size of the buffer.
 */
char *decimalToBinary(int p_val, char *buf, size_t num_bits) {
    unsigned val;
    size_t sz = num_bits + 1;
    assert(CHK(2)); /* at least two bytes of buffer space */
    buf += sz; /* we start from the end, backwards to avoid having to use
                * one bit masks moving all the time around */
    *--buf = '\0'; /* this is the last '\0' that should end the string */
    sz--; /* update buffer size */

    /* we operate better with unsigned, as the
     * sign doesn't get involved in shifts (we are reinterpreting
     * the sign bit as a normal bit, which makes the assumption that
     * integers are stored in two's complement.  This is essentially
     * nonportable code, but it will work in the stated assumptions. */
    val = (unsigned) p_val;

    /* the first below is the second char we check
     * above */
    do {
        *--buf = val & 1 ? '1' : '0';
        sz--;
        val >>= 1;
    } while (CHK(1) && val);
    while(sz) {
        *--buf = '0';
        sz--;
    }
    return buf; /* return what we have */
}

/**
 * Convert a binary word to a base32 word.
 *
 * @param binary_word a pointer to a string of binary digits (0's and 1's)
 * @param base32_word The base32 word to be returned.
 */
char *binaryToBase32Word(const char *binary_word, char *base32_word) {
    int msb_half = binaryToDecimal(binary_word, BINARY_WORD_SIZE / 2);
    int lsb_half = binaryToDecimal(binary_word + BINARY_WORD_SIZE / 2, BINARY_WORD_SIZE / 2);

    base32_word[0] = BASE32_DIGITS[msb_half];
    base32_word[1] = BASE32_DIGITS[lsb_half];
    base32_word[2] = '\0';

    return base32_word;
}

