//
// Created by misha on 17/08/2022.
//

#include <malloc.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "machine_code.h"
#include "linkedlist.h"
#include "parser.h"
#include "errors.h"
#include "const_tables.h"
#include "str_utils.h"
#include "symtab.h"
#include "base_conversion.h"

#define MAX_OPERANDS_COUNT 2

#define OPCODE_NUM_BITS 4
#define ADDRESSING_NUM_BITS 2
#define CODING_METHOD_NUM_BITS 2

#define REGISTER_NUM_BITS 4


struct machine_code_t {
    int line_num;
    int address; // 0-255

    int opcode; // 0-15
    int coding_method;  // A, E, R

    // first dest then src
    AddressingMode addressing_modes[MAX_OPERANDS_COUNT];

    int values[MAX_OPERANDS_COUNT]; // 0-255

    int registers[MAX_OPERANDS_COUNT]; // 0-15

    int label_addresses[MAX_OPERANDS_COUNT]; // 0-255

    const char *labels[MAX_OPERANDS_COUNT];

    int struct_addresses[MAX_OPERANDS_COUNT]; // 0-255
    int struct_field_nums[MAX_OPERANDS_COUNT]; // 1/2

    const char *struct_names[MAX_OPERANDS_COUNT];

    bool is_extern[MAX_OPERANDS_COUNT];
    int extern_words_index[MAX_OPERANDS_COUNT]; // 0-(size-1)

    int num_operands; // 0/1/2
    const char *operands[MAX_OPERANDS_COUNT];

    size_t size;
    char **words;
};


MachineCode machineCodeCreate(Statement s, int ic) {
    MachineCode mc = malloc(sizeof(*mc));
    if (!mc) {
        memoryAllocationError();
    }

    mc->line_num = statementGetLineNum(s);
    mc->address = ic;
    mc->opcode = getInstructionCode(statementGetMnemonic(s));
    mc->size = 1; // opcode word

    /* It's initializing the fields of the struct. */
    for (int i = 0; i < MAX_OPERANDS_COUNT; ++i) {
        mc->addressing_modes[i] = EMPTY_ADDRESSING;
        mc->values[i] = 0;
        mc->registers[i] = 0;
        mc->label_addresses[i] = 0;
        mc->struct_addresses[i] = 0;
        mc->struct_field_nums[i] = 0;
        mc->struct_names[i] = NULL;
        mc->labels[i] = NULL;
    }
    mc->words = NULL;

    List instruction_operands = statementGetOperands(s);
    int num_operands = listLength(instruction_operands);
    assert(num_operands == getInstructionNumberOfOperands(statementGetMnemonic(s)));

    mc->num_operands = num_operands;

    for (int i = 0; i < num_operands; ++i) {
        mc->size++; // operand value/address word

        const char *operand = listGetDataAt(instruction_operands, i);
        mc->operands[i] = strdup(operand);

        AddressingMode addressing_mode = getAddressingMode(operand);
        mc->addressing_modes[i] = addressing_mode;

        if (addressing_mode == IMMEDIATE_ADDRESSING) {
            mc->values[i] = atoi(operand + 1); // +1 to skip the '#'
        } else if (addressing_mode == REGISTER_ADDRESSING) {
            mc->registers[i] = atoi(operand + 1); // +1 to skip the 'r'
        } else if (addressing_mode == DIRECT_ADDRESSING) {
            mc->labels[i] = strdup(operand);
        } else if (addressing_mode == STRUCT_ADDRESSING) {
            mc->struct_names[i] = strdup(operand);
            mc->size++; // struct field num word

            List split_operand = strSplit(operand, ".");
            const char *after_delim = listGetDataAt(split_operand, 1);

            mc->struct_field_nums[i] = atoi(after_delim);
        }
        mc->is_extern[i] = false;
        mc->extern_words_index[i] = 0;
    }

    return mc;
}

int machineCodeCmp(MachineCode mc1, MachineCode mc2) {
    return mc1->address - mc2->address;
}

MachineCode machineCodeCopy(MachineCode mc) {
    MachineCode copy = malloc(sizeof(*copy));
    if (!copy) {
        memoryAllocationError();
    }
    copy->line_num = mc->line_num;
    copy->address = mc->address;
    copy->opcode = mc->opcode;
    copy->coding_method = mc->coding_method;
    copy->num_operands = mc->num_operands;
    copy->size = mc->size;
    copy->words = malloc(sizeof(char *) * copy->size);
    if (!copy->words) {
        memoryAllocationError();
    }
    for (int i = 0; i < copy->size; ++i) {
        copy->words[i] = strdup(mc->words[i]);
    }
    for (int i = 0; i < copy->num_operands; ++i) {
        copy->addressing_modes[i] = mc->addressing_modes[i];
        copy->values[i] = mc->values[i];
        copy->registers[i] = mc->registers[i];
        copy->label_addresses[i] = mc->label_addresses[i];
        copy->struct_addresses[i] = mc->struct_addresses[i];
        copy->struct_field_nums[i] = mc->struct_field_nums[i];
        copy->struct_names[i] = strdup(mc->struct_names[i]);
        copy->labels[i] = strdup(mc->labels[i]);
        copy->is_extern[i] = mc->is_extern[i];
        copy->extern_words_index[i] = mc->extern_words_index[i];
    }
    return copy;
}

void machineCodeDestroy(MachineCode mc) {
    if (mc->words) {
        for (int i = 0; i < mc->size; ++i) {
            free(mc->words[i]);
        }
        free(mc->words);
    }
    for (int i = 0; i < MAX_OPERANDS_COUNT; ++i) {
        free((void *) mc->labels[i]);
        free((void *) mc->struct_names[i]);
        free((void *) mc->operands[i]);
    }
    free(mc);
}

size_t machineCodeGetSize(MachineCode mc) {
    return mc->size;
}

static bool
updateAndCheckSymbolAddresses(MachineCode mc, List symtab, const char *filename, const char *filename_suffix,
                              int start_address) {
    bool success = true;
    for (int i = 0; i < mc->num_operands; ++i) {
        if (mc->addressing_modes[i] == DIRECT_ADDRESSING) {
            SymtabEntry found_entry = symbolTableFindByName(symtab, mc->labels[i]);
            if (!found_entry) {
                success = false;
                printf("Undefined symbol %s on line %d in file %s%s\n", mc->labels[i], mc->line_num, filename,
                       filename_suffix);
            }
            mc->label_addresses[i] = symtabEntryGetValue(found_entry) + start_address;
            mc->is_extern[i] = symtabEntryGetType(found_entry) == SYMBOL_EXTERN;

        } else if (mc->addressing_modes[i] == STRUCT_ADDRESSING) {
            SymtabEntry found_entry = symbolTableFindByName(symtab, mc->struct_names[i]);
            if (!found_entry) {
                success = false;
                printf("Undefined symbol %s on line %d in file %s%s\n", mc->labels[i], mc->line_num, filename,
                       filename_suffix);
            }
            mc->struct_addresses[i] = symtabEntryGetValue(found_entry) + start_address;
            mc->is_extern[i] = symtabEntryGetType(found_entry) == SYMBOL_EXTERN;
        }
    }
    return success;
}

/**
 * It converts the machine code to base 32 words.
 *
 * @param mc a list of machine code instructions
 * @param symtab a list of symbols (strings)
 *
 * @return true if successful, false otherwise
 */
bool machineCodeUpdateFromSymtab(MachineCode mc, List symtab, const char *filename_suffix, const char *filename,
                                 int start_address_offset) {
    if (!updateAndCheckSymbolAddresses(mc, symtab, filename, filename_suffix, start_address_offset)) {
        return false;
    }

    char **words = malloc(sizeof(*words) * mc->size);
    if (!words) {
        memoryAllocationError();
    }
    for (int i = 0; i < mc->size; ++i) {
        words[i] = malloc(sizeof(char) * (BASE32_WORD_SIZE + 1));
        if (!words[i]) {
            memoryAllocationError();
        }
    }
    mc->words = words;

    char binary_buf[BINARY_WORD_SIZE + 1];
    // opcode word
    decimalToBinary(mc->opcode, binary_buf, OPCODE_NUM_BITS);
    decimalToBinary(mc->addressing_modes[0], binary_buf + OPCODE_NUM_BITS,
                    ADDRESSING_NUM_BITS);
    decimalToBinary(mc->addressing_modes[1], binary_buf + OPCODE_NUM_BITS + ADDRESSING_NUM_BITS,
                    ADDRESSING_NUM_BITS);
    decimalToBinary(A, binary_buf + OPCODE_NUM_BITS + 2 * ADDRESSING_NUM_BITS, CODING_METHOD_NUM_BITS);
    binaryToBase32Word(binary_buf, words[0]);

    // operand value/address word(s)
    if (mc->addressing_modes[0] == REGISTER_ADDRESSING && mc->addressing_modes[1] == REGISTER_ADDRESSING) {
        assert(mc->size == 2);
        decimalToBinary(mc->registers[0], binary_buf, REGISTER_NUM_BITS);
        decimalToBinary(mc->registers[1], binary_buf + REGISTER_NUM_BITS, REGISTER_NUM_BITS);
        binaryToBase32Word(binary_buf, words[1]);
        return true;
    }

    int operand_word_index = 1;
    for (int i = 0; i < mc->num_operands; ++i) {
        if (mc->addressing_modes[i] == IMMEDIATE_ADDRESSING) {
            decimalToBinary(mc->values[i], binary_buf, BINARY_WORD_SIZE - 2);
            decimalToBinary(A, binary_buf + OPCODE_NUM_BITS + 2 * ADDRESSING_NUM_BITS, CODING_METHOD_NUM_BITS);
            binaryToBase32Word(binary_buf, words[operand_word_index++]);
        } else if (mc->addressing_modes[i] == REGISTER_ADDRESSING) {
            decimalToBinary(mc->registers[i], binary_buf + i * REGISTER_NUM_BITS, REGISTER_NUM_BITS);
            decimalToBinary(A, binary_buf + OPCODE_NUM_BITS + 2 * ADDRESSING_NUM_BITS, CODING_METHOD_NUM_BITS);
            binaryToBase32Word(binary_buf, words[operand_word_index++]);
        } else if (mc->addressing_modes[i] == DIRECT_ADDRESSING) {
            if (mc->is_extern[i]) {
                decimalToBinary(0, binary_buf, BINARY_WORD_SIZE - 2);
                decimalToBinary(E, binary_buf + OPCODE_NUM_BITS + 2 * ADDRESSING_NUM_BITS, CODING_METHOD_NUM_BITS);
                mc->extern_words_index[i] = operand_word_index;
            } else {
                decimalToBinary(mc->label_addresses[i], binary_buf, BINARY_WORD_SIZE - 2);
                decimalToBinary(R, binary_buf + OPCODE_NUM_BITS + 2 * ADDRESSING_NUM_BITS, CODING_METHOD_NUM_BITS);
            }
            binaryToBase32Word(binary_buf, words[operand_word_index++]);
        } else if (mc->addressing_modes[i] == STRUCT_ADDRESSING) {
            if (mc->is_extern[i]) {
                decimalToBinary(0, binary_buf, BINARY_WORD_SIZE - 2);
                decimalToBinary(E, binary_buf + OPCODE_NUM_BITS + 2 * ADDRESSING_NUM_BITS, CODING_METHOD_NUM_BITS);
                mc->extern_words_index[i] = operand_word_index;
            } else {
                decimalToBinary(mc->struct_addresses[i], binary_buf, BINARY_WORD_SIZE - 2);
                decimalToBinary(R, binary_buf + OPCODE_NUM_BITS + 2 * ADDRESSING_NUM_BITS, CODING_METHOD_NUM_BITS);
            }
            binaryToBase32Word(binary_buf, words[operand_word_index++]);
            decimalToBinary(mc->struct_field_nums[i], binary_buf, BINARY_WORD_SIZE - 2);
            decimalToBinary(A, binary_buf + OPCODE_NUM_BITS + 2 * ADDRESSING_NUM_BITS, CODING_METHOD_NUM_BITS);
            binaryToBase32Word(binary_buf, words[operand_word_index++]);
        }
    }
    assert(operand_word_index == mc->size);
    return true;
}

int machineCodeGetNumOperands(MachineCode mc) {
    return mc->num_operands;
}

const char *machineCodeGetOperand(MachineCode mc, int index) {
    return mc->operands[index];
}

bool machineCodeGetIsExternOperand(MachineCode mc, int index) {
    return mc->is_extern[index];
}

const char *machineCodeGetExternalOperandBase32Address(MachineCode mc, int index) {
    if (!mc->is_extern[index]) {
        return NULL;
    }
    return mc->words[mc->extern_words_index[index]];
}

void machineCodeToObjFile(MachineCode mc, FILE *f, int start_address_offset) {
    // TODO: implement
}

