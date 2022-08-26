//
// Created by misha on 30/07/2022.
//

#include "parser.h"
#include "str_utils.h"
#include "errors.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <assert.h>
#include "const_tables.h"
#include "rules.h"
#include "types_utils.h"

#define WHITESPACE_DELIM " \t\n "
#define OPERANDS_DELIM ","
#define OPERANDS_DELIM_CHAR ','
#define LABEL_SUFFUX ":"
#define COMMENT_PREFIX ";"

#define START_MACRO_STR "macro"
#define END_MACRO_STR "endmacro"

#define LABEL_MAX_LENGTH 30

struct statement_t {
    int line_num;
    const char *raw_text;
    List tokens;

    StatementType type;
    const char *label;
    const char *mnemonic;
    List operands;
};


/**
 * It checks if the line is a comment line
 *
 * @param line The line to check.
 * @return true if the line is a comment line, false otherwise.
 */
static bool isCommentLine(const char *line) {
    return strStartsWith(line, COMMENT_PREFIX, true);
}

/**
 * It checks if a string is a label
 *
 * @param s the string to check
 *
 * @return true if the string is a label, false otherwise.
 */
static bool isLabel(const char *s) {
    return strEndsWith(s, LABEL_SUFFUX);
}

/**
 * It checks if the string is a directive.
 *
 * @param str The string to check.
 *
 * @return true if the string is a directive, false otherwise.
 */
static bool isDirective(const char *str) {
    for (int i = 0; i < DIRECTIVES_SIZE; ++i) {
        if (strcmp(str, DIRECTIVES[i]) == 0)
            return true;
    }
    return false;
}

/**
 * It checks if the directive is a data store directive. I.e., it checks if the directive is .data, .string or .struct
 *
 * @param directive The directive to check.
 */
bool isDataStoreDirective(const char *directive) {
    return strcmp(directive, DIRECTIVE_DATA) == 0 || strcmp(directive, DIRECTIVE_STRING) == 0 ||
           strcmp(directive, DIRECTIVE_STRUCT) == 0;
}

/**
 * It checks if the string is an instruction.
 *
 * @param str The string to check
 *
 * @return true if the string is an instruction, false otherwise.
 */
static bool isInstruction(const char *str) {
    for (int i = 0; i < INSTRUCTIONS_ALL_SIZE; ++i) {
        if (strcmp(str, INSTRUCTIONS_ALL[i]) == 0)
            return true;
    }
    return false;
}

/**
 * It returns true if the string passed in is the start macro string
 *
 * @param s The string to check
 *
 * @return true if the string is the start macro string, false otherwise.
 */
static bool isMacroStart(const char *s) {
    return strcmp(s, START_MACRO_STR) == 0;
}

/**
 * It returns true if the string passed to it is the string "END_MACRO"
 *
 * @param s The string to check.
 *
 * @return true if the string is the end macro string, false otherwise.
 */
static bool isMacroEnd(const char *s) {
    return strcmp(s, END_MACRO_STR) == 0;
}

/**
 * It parses a line of text into a Statement.
 *
 * @param line The line of text to parse.
 */
Statement parse(const char *line, int line_num) {
    if (isCommentLine(line)) {
        return statementCreate(line_num, COMMENT, line, NULL, NULL, NULL, NULL);
    }

    const char *line_replaced = strReplace(line, OPERANDS_DELIM, WHITESPACE_DELIM);
    if (!line_replaced) {
        memoryAllocationError();
    }

    List tokens = strSplit(line_replaced, WHITESPACE_DELIM);
    /* Checking that the list of tokens is not empty. */
    if (listLength(tokens) == 0) { // empty line
        listDestroy(tokens);
        free((char *) line_replaced);
        return statementCreate(line_num, EMPTY_LINE, line, NULL, NULL, NULL, NULL);
    }

    free((char *) line_replaced);

    int token_index = 0;
    const char *token = listGetDataAt(tokens, token_index);

    const char *label = NULL;
    if (isLabel(token)) {
        label = strndup(token, strlen(token) - strlen(LABEL_SUFFUX));
        token_index++;
        token = listGetDataAt(tokens, token_index);
    }
    StatementType type;
    if (isMacroStart(token)) { // && listLength(tokens) == 2) {
        type = MACRO_START;
    } else if (isMacroEnd(token)) { // && listLength(tokens) == 1) {
        type = MACRO_END;
    } else if (isDirective(token)) {
        type = DIRECTIVE;
    } else if (isInstruction(token)) {
        type = INSTRUCTION;
    } else {
        type = OTHER;
    }

    const char *mnemonic = token;

    token_index++;
    List operands = listCopyFromIndex(tokens, token_index);

    Statement s = statementCreate(line_num, type, line, label, mnemonic, operands, tokens);
    free((char *) label);
    listDestroy(tokens);
    listDestroy(operands);

    return s;
}

/**
 * It creates a statement.
 *
 * @param type The type of statement.
 * @param raw_text The raw text of the statement.
 * @param label The label of the statement, if any.
 * @param mnemonic The mnemonic of the instruction.
 * @param operands A list of operands. Each operand is a list of tokens.
 * @param tokens A list of tokens that make up the statement.
 */
Statement
statementCreate(int line_num, StatementType type, const char *raw_text, const char *label, const char *mnemonic,
                List operands,
                List tokens) {
    Statement s = (Statement) malloc(sizeof(*s));
    if (!s)
        memoryAllocationError();

    s->line_num = line_num;
    s->type = type;
    s->raw_text = strdup(raw_text);
    s->label = strdup(label);
    s->mnemonic = strdup(mnemonic);

    s->operands = listCopy(operands);
    s->tokens = listCopy(tokens);

    return s;
}

/**
 * It destroys the statement.
 *
 * @param s The statement to destroy.
 */
void statementDestroy(Statement s) {
    free((char *) s->raw_text);
    free((char *) s->label);
    free((char *) s->mnemonic);
    listDestroy(s->operands);
    listDestroy(s->tokens);
    free(s);
}

/**
 * It returns the line number of the statement.
 *
 * @param s The statement to get the line number of.
 */
int statementGetLineNum(Statement s) {
    return s->line_num;
}

/**
 * It returns the type of the statement.
 *
 * @param s The statement to get the type of.
 */
StatementType statementGetType(Statement s) {
    return s->type;
}

/**
 * It returns the raw text of the statement.
 *
 * @param s The statement object
 */
const char *statementGetRawText(Statement s) {
    return s->raw_text;
}

/**
 * It returns the label of the statement.
 *
 * @param s The statement to get the label of.
 */
const char *statementGetLabel(Statement s) {
    return s->label;
}

/**
 * It returns the mnemonic of the statement.
 *
 * @param s The statement to get the mnemonic for.
 */
const char *statementGetMnemonic(Statement s) {
    return s->mnemonic;
}

/**
 * It returns the operands of the statement.
 *
 * @param s The statement to get the operands of.
 */
const List statementGetOperands(Statement s) {
    return s->operands;
}

/**
 * It returns the list of tokens in the statement.
 *
 * @param s The statement to get the tokens from.
 */
const List statementGetTokens(Statement s) {
    return s->tokens;
}

/**
 * It checks the syntax of the label.
 *
 * @param s The statement to check.
 */
static bool labelCheckSyntax(Statement s, const char *filename, const char *filename_suffix) {
    assert(s->label != NULL);

    if (strlen(s->label) > LABEL_MAX_LENGTH) {
        printf("Error in %s.%s line %d: Label is too long\n", filename, filename_suffix, s->line_num);
        return false;
    }
    if (!isalpha(s->label[0])) {
        printf("Error in %s.%s line %d: Label must start with a letter\n", filename, filename_suffix, s->line_num);
        return false;
    }
    for (int i = 1; i < strlen(s->label); i++) {
        if (!isalnum(s->label[i])) {
            printf("Error in %s.%s line %d: Label must contain only letters and digits\n", filename, filename_suffix,
                   s->line_num);
            return false;
        }
    }
    if (s->type != DIRECTIVE && s->type != INSTRUCTION) {
        printf("Error in %s.%s line %d: Label can only be used with directives and instructions\n", filename,
               filename_suffix, s->line_num);
        return false;
    }
    if (s->type == DIRECTIVE && !isDataStoreDirective(s->mnemonic)) {
        printf("Error in %s.%s line %d: Label can only be used with directives data, struct and string\n", filename,
               filename_suffix, s->line_num);
        return false;
    }
    if (isReservedWord(s->label)) {
        printf("Error in %s.%s line %d: Label can not be a reserved word\n", filename, filename_suffix, s->line_num);
        return false;
    }
    return true;
}

/**
 * It checks the syntax of the macro.
 *
 * @param s The statement to check.
 */
static bool macroCheckSyntax(Statement s, const char *filename, const char *filename_suffix) {
    assert(s->type == MACRO_START);

    if (s->label != NULL) {
        printf("Error in %s%s line %d: Macro can not have a label\n", filename, filename_suffix, s->line_num);
        return false;
    }
    if (listLength(s->operands) != 1) {
        printf("Error in %s%s line %d: Macro start statement must have exactly one argument\n", filename,
               filename_suffix, s->line_num);
        return false;
    }
    const char *macro_name = listGetDataAt(s->operands, 0);
    if (isDirective(macro_name) || isInstruction(macro_name)) {
        printf("Error in %s%s line %d: Macro name can't be an instruction or a directive!\n", filename, filename_suffix,
               s->line_num);
        return false;
    }
    if (strlen(macro_name) > LABEL_MAX_LENGTH) {
        printf("Error in %s%s line %d: Macro name is too long\n", filename, filename_suffix, s->line_num);
        return false;
    }
    if (!isalpha(macro_name[0])) {
        printf("Error in %s%s line %d: Macro name must start with a letter\n", filename, filename_suffix, s->line_num);
        return false;
    }
    for (int i = 1; i < strlen(macro_name); i++) {
        if (!isalnum(macro_name[i])) {
            printf("Error in %s%s line %d: Macro name must contain only letters and digits\n", filename,
                   filename_suffix, s->line_num);
            return false;
        }
    }
    return true;
}

/**
 * It checks the syntax of a directive statement
 *
 * @param s The statement that is being checked.
 * @param filename the name of the file being parsed
 * @param filename_suffix The suffix of the file name. For example, if the file name is "test.c", the suffix is "c".
 */
static bool directiveCheckSyntax(Statement s, const char *filename, const char *filename_suffix) {
    if (listLength(s->operands) == 0) {
        printf("Error in %s%s line %d: Directive must have at least one argument\n", filename, filename_suffix,
               s->line_num);
        return false;
    }
    if (strcmp(s->mnemonic, DIRECTIVE_DATA) == 0) {
        for (int i = 0; i < listLength(s->operands); i++) {
            const char *operand = listGetDataAt(s->operands, i);
            if (!isNumeric(operand)) {
                printf("Error in %s%s line %d: Directive .data must be numeric\n", filename, filename_suffix,
                       s->line_num);
                return false;
            }
        }
    } else if (strcmp(s->mnemonic, DIRECTIVE_STRING) == 0) {
        if (listLength(s->operands) != 1) {
            printf("Error in %s%s line %d: Directive .string must have exactly one argument\n", filename,
                   filename_suffix, s->line_num);
            return false;
        }
        const char *operand = listGetDataAt(s->operands, 0);
        if (!isString(operand)) {
            printf("Error in %s%s line %d: Directive .string operand must be a string\n", filename, filename_suffix,
                   s->line_num);
            return false;
        }
    } else if (strcmp(s->mnemonic, DIRECTIVE_STRUCT) == 0) {
        if (listLength(s->operands) != 2) {
            printf("Error in %s%s line %d: Directive .struct must have exactly two arguments\n", filename,
                   filename_suffix, s->line_num);
            return false;
        }
        bool res = true;
        if (!isNumeric(listGetDataAt(s->operands, 0))) {
            printf("Error in %s%s line %d: Directive .struct first argument must be numeric\n", filename,
                   filename_suffix, s->line_num);
            res = false;
        }
        if (!isString(listGetDataAt(s->operands, 1))) {
            printf("Error in %s%s line %d: Directive .struct second argument must be a string\n", filename,
                   filename_suffix, s->line_num);
            res = false;
        }
        return res;
    } else if (strcmp(s->mnemonic, DIRECTIVE_ENTRY) == 0) {
        if (listLength(s->operands) != 1) {
            printf("Error in %s%s line %d: Directive .entry must have exactly one argument\n", filename,
                   filename_suffix, s->line_num);
            return false;
        }
    } else if (strcmp(s->mnemonic, DIRECTIVE_EXTERN) == 0) {
        if (listLength(s->operands) != 1) {
            printf("Error in %s%s line %d: Directive .extern must have exactly one argument\n", filename,
                   filename_suffix, s->line_num);
            return false;
        }
    }
    return true;
}

/**
 * It checks the syntax of the instruction.
 *
 * @param s The statement to check.
 * @param filename The name of the file that the statement is in.
 * @param filename_suffix The suffix of the file that is being checked.
 */
static bool instructionCheckSyntax(Statement s, const char *filename, const char *filename_suffix) {
    if (is0OperandInstruction(s->mnemonic) && listLength(s->operands) != 0) {
        printf("Error in %s%s line %d: Instruction %s must have no operands\n", filename, filename_suffix, s->line_num,
               s->mnemonic);
        return false;
    } else if (is1OperandInstruction(s->mnemonic) && listLength(s->operands) != 1) {
        printf("Error in %s%s line %d: Instruction %s must have exactly one operand\n", filename, filename_suffix,
               s->line_num, s->mnemonic);
        return false;
    } else if (is2OperandInstruction(s->mnemonic) && listLength(s->operands) != 2) {
        printf("Error in %s%s line %d: Instruction %s must have exactly two operands\n", filename, filename_suffix,
               s->line_num, s->mnemonic);
        return false;
    }
    int num_operands = listLength(s->operands);
    for (int i = 0; i < num_operands; i++) {
        const char *operand = listGetDataAt(s->operands, i);
        AddressingMode mode = getAddressingMode(operand);
        if (mode == INVALID_ADDRESSING) {
            printf("Error in %s%s line %d: operand %s is not a valid operand\n", filename, filename_suffix, s->line_num,
                   operand);
            return false;
        }
    }
    if (num_operands == 1) {
        const char *operand = listGetDataAt(s->operands, 0);
        AddressingMode mode = getAddressingMode(operand);
        if (!isValidAddressing_1_OP(s->mnemonic, mode)) {
            printf("Error in %s%s line %d: invalid addressing for operand %s and instruction %s\n", filename,
                   filename_suffix, s->line_num, operand, s->mnemonic);
            return false;
        }
    } else if (num_operands == 2) {
        const char *operand_src = listGetDataAt(s->operands, 0);
        const char *operand_dest = listGetDataAt(s->operands, 1);
        AddressingMode mode_src = getAddressingMode(operand_src);
        AddressingMode mode_dest = getAddressingMode(operand_dest);

        if (!isValidAddressing_2_OP(s->mnemonic, mode_src, mode_dest)) {
            printf("Error in %s%s line %d: invalid addressing for operands %s and %s and instruction %s\n", filename,
                   filename_suffix, s->line_num, operand_src, operand_dest, s->mnemonic);
            return false;
        }
    }
    return true;
}

/**
 * It checks the syntax of the delimiters.
 *
 * @param s The statement to check.
 * @param filename the name of the file being parsed
 * @param filename_suffix The suffix of the file name.
 */
static bool delimiterCheckSyntax(Statement s, const char *filename, const char *filename_suffix) {
    if (listLength(s->operands) == 0) {
        if (strCountChar(s->raw_text, OPERANDS_DELIM_CHAR) > 0) {
            printf("Error in %s%s line %d: number of operands does not match number of delimiters\n", filename,
                   filename_suffix,
                   s->line_num);
            return false;
        }
    } else {
        if (strCountChar(s->raw_text, OPERANDS_DELIM_CHAR) != listLength(s->operands) - 1) {
            printf("Error in %s%s line %d: number of operands does not match number of delimiters\n", filename,
                   filename_suffix,
                   s->line_num);
            return false;
        }
        const char *with_mnemonic;
        if (s->label) {
            with_mnemonic = strReplace(s->raw_text, s->label, WHITESPACE_DELIM);
        } else {
            with_mnemonic = s->raw_text;
        }
        const char *only_operands = strReplace(with_mnemonic, s->mnemonic, WHITESPACE_DELIM);
        List seperated_by_delim = strSplit(only_operands, OPERANDS_DELIM);
        if (listLength(seperated_by_delim) != listLength(s->operands)) {
            printf("Error in %s%s line %d: misplaced delimiters\n", filename, filename_suffix,
                   s->line_num);
            return false;
        }
        free((void *) with_mnemonic);
        free((void *) only_operands);
        listDestroy(seperated_by_delim);
    }
    return true;
}

/**
 * It checks the syntax of the statement.
 *
 * @param s The statement to check.
 * @param filename the name of the file being checked
 * @param filename_suffix The suffix of the file name. For example, if the file name is "test.c", the suffix is ".c".
 */
bool statementCheckSyntax(Statement s, const char *filename, const char *filename_suffix) {
    if (!s) {
        return false;
    }

    if (s->type == OTHER) {
        printf("Error in %s%s line %d: Undefined/Invalid statement\n", filename, filename_suffix, s->line_num);
        return false;
    }
    if (s->type == COMMENT || s->type == EMPTY_LINE) {
        return true;
    }

    if (s->type == MACRO_START) {
        return macroCheckSyntax(s, filename, filename_suffix);
    } else if (s->type == MACRO_END) {
        if (listLength(s->operands) != 0) {
            printf("Error in %s%s line %d: Macro end statement must have no arguments\n", filename, filename_suffix,
                   s->line_num);
            return false;
        }
        return true;
    } else {  // directive or instruction
        bool valid = delimiterCheckSyntax(s, filename, filename_suffix);
        if (s->label) {
            valid = valid && labelCheckSyntax(s, filename, filename_suffix);
        }
        if (s->type == DIRECTIVE) {
            valid = valid && directiveCheckSyntax(s, filename, filename_suffix);
        } else { // instruction
            valid = valid && instructionCheckSyntax(s, filename, filename_suffix);
        }
        return valid;
    }
}
