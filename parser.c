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
#define QUOTATION_CHAR '"'

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
 * It checks the syntax of the label.
 *
 * @param s The statement to check.
 */
static bool labelCheckSyntax(const char *label, int line_num, const char *filename, const char *filename_suffix) {
    assert(label != NULL);

    if (strlen(label) > LABEL_MAX_LENGTH) {
        printf("Error in %s%s line %d: Label is too long\n", filename, filename_suffix, line_num);
        return false;
    }
    if (!isalpha(label[0])) {
        printf("Error in %s%s line %d: Label must start with a letter\n", filename, filename_suffix, line_num);
        return false;
    }
    if (!isAlphaNumeric(label)) {
        printf("Error in %s%s line %d: Label must contain only letters and digits\n", filename, filename_suffix,
               line_num);
        return false;
    }
    if (isReservedWord(label)) {
        printf("Error in %s%s line %d: Label can not be a reserved word\n", filename, filename_suffix, line_num);
        return false;
    }
    return true;
}

/**
 * It checks the syntax of the macro.
 *
 * @param macro_name The statement to check.
 */
static bool macroCheckSyntax(const char *macro_name, int line_num, const char *filename, const char *filename_suffix) {
    int i;
    if (isReservedWord(macro_name)) {
        printf("Error in %s%s line %d: Macro name can't be a reserved word\n", filename, filename_suffix,
               line_num);
        return false;
    }
    if (strlen(macro_name) > LABEL_MAX_LENGTH) {
        printf("Error in %s%s line %d: Macro name is too long\n", filename, filename_suffix, line_num);
        return false;
    }
    if (!isalpha(macro_name[0])) {
        printf("Error in %s%s line %d: Macro name must start with a letter\n", filename, filename_suffix, line_num);
        return false;
    }
    for (i = 1; i < strlen(macro_name); i++) {
        if (!isalnum(macro_name[i])) {
            printf("Error in %s%s line %d: Macro name must contain only letters and digits\n", filename,
                   filename_suffix, line_num);
            return false;
        }
    }
    return true;
}

/**
 * It checks if the line is a comment line
 *
 * @param line The line to check.
 * @return true if the line is a comment line, false otherwise.
 */
static bool isCommentLine(const char *line) {
    if (!line) {
        return false;
    }
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
    if (!s) {
        return false;
    }
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
    int i;
    if (!str) {
        return false;
    }
    for (i = 0; i < DIRECTIVES_SIZE; ++i) {
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
    if (!directive) {
        return false;
    }
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
    int i;
    if (!str) {
        return false;
    }
    for (i = 0; i < INSTRUCTIONS_ALL_SIZE; ++i) {
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
    if (!s) {
        return false;
    }
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
    if (!s) {
        return false;
    }
    return strcmp(s, END_MACRO_STR) == 0;
}


void nextTokenInLine(const char *line, int *next_token_start_idx_ptr, int *next_token_stop_idx_ptr) {
    *next_token_start_idx_ptr = strFindNextNonWhitespace(line, *next_token_stop_idx_ptr);
    *next_token_stop_idx_ptr = strFindNextWhitespace(line, *next_token_start_idx_ptr);
}

void nextOperandInLine(const char *line, int *next_operand_start_idx_ptr, int *next_operand_stop_idx_ptr) {
    *next_operand_start_idx_ptr = strFindNextNonWhitespaceAndDelimiter(line, *next_operand_stop_idx_ptr, OPERANDS_DELIM_CHAR);
    *next_operand_stop_idx_ptr = strFindNextWhitespaceOrDelimiter(line, *next_operand_start_idx_ptr, OPERANDS_DELIM_CHAR);
}


/**
 * It parses a line of text into a Statement.
 *
 * @param line The line of text to parse.
 */
Statement
parse(const char *line, int line_num, const char *filename, const char *filename_suffix, bool check_non_macro_syntax) {
    const char *label = NULL;
    bool success = true;
    int next_token_start_idx = 0, next_token_stop_idx = 0;

    List tokens;
    char *token;
    List operands;
    StatementType type;
    const char *mnemonic;
    int prev_token_stop_idx;
    int string_start_idx, string_end_idx;
    int operand_id;
    Statement s;

    /* skip if a comment line */
    if (isCommentLine(line)) {
        return statementCreate(line_num, COMMENT, line, NULL, NULL, NULL, NULL);
    }
    /* check if an empty line, if so then skip */
    nextTokenInLine(line, &next_token_start_idx, &next_token_stop_idx);
    if (next_token_start_idx == -1) {
        return statementCreate(line_num, EMPTY_LINE, line, NULL, NULL, NULL, NULL);
    }
    /* read the first token */
    tokens = listCreate((list_eq) strcmp, (list_copy) strdup, (list_free) free);
    token = strndup(line + next_token_start_idx, next_token_stop_idx - next_token_start_idx);
    if (!token) {
        memoryAllocationError();
    }
    listAppend(tokens, token);

    if (isLabel(token)) {
        label = strndup(token, strlen(token) - strlen(LABEL_SUFFUX));
        if (!label) {
            memoryAllocationError();
        }
        nextTokenInLine(line, &next_token_start_idx, &next_token_stop_idx);
        if (next_token_start_idx == -1) {
            free(token);
            token = NULL;
            success = false;
        } else {
            free(token);
            token = strndup(line + next_token_start_idx, next_token_stop_idx - next_token_start_idx);
            if (!token) {
                memoryAllocationError();
            }
            listAppend(tokens, token);
        }
    }
    if (isMacroStart(token)) {
        type = MACRO_START;
    } else if (isMacroEnd(token)) {
        type = MACRO_END;
    } else if (isDirective(token)) {
        type = DIRECTIVE;
    } else if (isInstruction(token)) {
        type = INSTRUCTION;
    } else {
        type = OTHER;
    }

    if (check_non_macro_syntax && label && !labelCheckSyntax(label, line_num, filename, filename_suffix)) {
        success = false;
    }

    mnemonic = strdup(token);
    operands = listCreate((list_eq) strcmp, (list_copy) strdup, (list_free) free);
    prev_token_stop_idx = next_token_stop_idx;

    if (type == OTHER && check_non_macro_syntax) {
        printf("Error in %s%s line %d: Unknown directive/instruction, undefined statement\n", filename, filename_suffix,
               line_num);
        success = false;
    } else if (type == MACRO_START) {
        if (label) {
            printf("Error in %s%s line %d: Macro start statement can not have a label\n", filename, filename_suffix,
                   line_num);
            success = false;
        }
        if (strCountCharInRange(line, OPERANDS_DELIM_CHAR, 0, next_token_start_idx) > 0) {
            printf("Error in %s%s line %d: Misplaced comma delimiter before macro start statement\n", filename,
                   filename_suffix, line_num);
            success = false;
        }
        nextOperandInLine(line, &next_token_start_idx, &next_token_stop_idx);
        if (next_token_start_idx == -1) {
            printf("Error in %s%s line %d: Macro start statement must have exactly one argument\n", filename,
                   filename_suffix, line_num);
            success = false;
        }
        if (strCountCharInRange(line, OPERANDS_DELIM_CHAR, prev_token_stop_idx, next_token_start_idx) > 0) {
            printf("Error in %s%s line %d: Misplaced comma delimiter before macro name\n", filename,
                   filename_suffix, line_num);
            success = false;
        }
        if (success) {
            free(token);
            token = strndup(line + next_token_start_idx, next_token_stop_idx - next_token_start_idx);
            if (!token) {
                memoryAllocationError();
            }
            listAppend(tokens, token);
            listAppend(operands, token);
        }

        prev_token_stop_idx = next_token_stop_idx;
        nextOperandInLine(line, &next_token_start_idx, &next_token_stop_idx);
        if (next_token_start_idx != -1) {
            printf("Error in %s%s line %d: Macro start statement must have exactly one argument\n", filename,
                   filename_suffix, line_num);
            success = false;
        }
        if (strCountCharInRange(line, OPERANDS_DELIM_CHAR, prev_token_stop_idx, strlen(line)) > 0) {
            printf("Error in %s%s line %d: Misplaced comma delimiter after macro name\n", filename,
                   filename_suffix, line_num);
            success = false;
        }
        macroCheckSyntax(token, line_num, filename, filename_suffix);
    } else if (type == MACRO_END) {
        if (label) {
            printf("Error in %s%s line %d: Macro end statement can not have a label\n", filename, filename_suffix,
                   line_num);
            success = false;
        }
        if (strCountCharInRange(line, OPERANDS_DELIM_CHAR, 0, next_token_start_idx) > 0) {
            printf("Error in %s%s line %d: Misplaced comma delimiter before macro end statement\n", filename,
                   filename_suffix, line_num);
            success = false;
        }
        prev_token_stop_idx = next_token_stop_idx;
        nextOperandInLine(line, &next_token_start_idx, &next_token_stop_idx);
        if (next_token_start_idx != -1) {
            printf("Error in %s%s line %d: Macro end statement must have no arguments\n", filename, filename_suffix,
                   line_num);
            success = false;
        }
        if (strCountCharInRange(line, OPERANDS_DELIM_CHAR, prev_token_stop_idx, strlen(line)) > 0) {
            printf("Error in %s%s line %d: Misplaced comma delimiter after macro end statement\n", filename,
                   filename_suffix, line_num);
            success = false;
        }
    } else if (type == DIRECTIVE && check_non_macro_syntax) {
        prev_token_stop_idx = next_token_stop_idx;
        nextOperandInLine(line, &next_token_start_idx, &next_token_stop_idx);
        if (next_token_start_idx == -1) {
            printf("Error in %s%s line %d: Directive must have at least one argument\n", filename, filename_suffix,
                   line_num);
            success = false;
        } else {
            free(token);
            token = strndup(line + next_token_start_idx, next_token_stop_idx - next_token_start_idx);
            if (!token) {
                memoryAllocationError();
            }
            listAppend(tokens, token);
            listAppend(operands, token);
        }
        if (strCountCharInRange(line, OPERANDS_DELIM_CHAR, prev_token_stop_idx, next_token_start_idx) > 0) {
            printf("Error in %s%s line %d: Misplaced comma delimiter before first operand\n", filename,
                   filename_suffix, line_num);
            success = false;
        }

        operand_id = 1;

        if (strcmp(mnemonic, DIRECTIVE_DATA) == 0) {
            while (next_token_start_idx != -1) {
                if (!isNumeric(token)) {
                    printf("Error in %s%s line %d: .data operand no. %d is not numeric\n", filename, filename_suffix,
                           line_num, operand_id);
                    success = false;
                }

                prev_token_stop_idx = next_token_stop_idx;
                nextOperandInLine(line, &next_token_start_idx, &next_token_stop_idx);
                if (next_token_start_idx == -1) {
                    break;
                }
                operand_id++;
                if (strCountCharInRange(line, OPERANDS_DELIM_CHAR, prev_token_stop_idx, next_token_start_idx) != 1) {
                    printf("Error in %s%s line %d: Must have exactly one comma delimiter between .data operands no. %d and no. %d\n",
                           filename,
                           filename_suffix, line_num, operand_id - 1, operand_id);
                    success = false;
                } else {
                    free(token);
                    token = strndup(line + next_token_start_idx, next_token_stop_idx - next_token_start_idx);
                    if (!token) {
                        memoryAllocationError();
                    }
                    listAppend(tokens, token);
                    listAppend(operands, token);
                }
            }
        } else if (strcmp(mnemonic, DIRECTIVE_STRING) == 0) {
            string_start_idx = next_token_start_idx;
            if (line[string_start_idx] != QUOTATION_CHAR) {
                printf("Error in %s%s line %d: .string directive operand must start with a \" character\n", filename,
                       filename_suffix, line_num);
                success = false;
            }
            string_end_idx = strFindNextChar(line, string_start_idx + 1, QUOTATION_CHAR);
            if (string_end_idx == -1) {
                printf("Error in %s%s line %d: .string directive operand must end with a \" character\n", filename,
                       filename_suffix, line_num);
                success = false;
            }
            if (success) {
                free(token);
                token = strndup(line + string_start_idx, string_end_idx - string_start_idx + 1);
                if (!token) {
                    memoryAllocationError();
                }
                listAppend(tokens, token);
                listAppend(operands, token);
            }
            next_token_start_idx = string_start_idx;
            next_token_stop_idx = string_end_idx + 1;
            nextOperandInLine(line, &next_token_start_idx, &next_token_stop_idx);
            if (next_token_start_idx != -1) {
                printf("Error in %s%s line %d: .string directive must have exactly one operand\n", filename,
                       filename_suffix, line_num);
                success = false;
            }
        } else if (strcmp(mnemonic, DIRECTIVE_STRUCT) == 0) {
            if (!isNumeric(token)) {
                printf("Error in %s%s line %d: .struct directive first operand must be numeric\n", filename,
                       filename_suffix, line_num);
                success = false;
            }
            prev_token_stop_idx = next_token_stop_idx;

            string_start_idx = strFindNextChar(line, next_token_stop_idx, QUOTATION_CHAR);

            if (strCountCharInRange(line, OPERANDS_DELIM_CHAR, prev_token_stop_idx, string_start_idx) != 1) {
                printf("Error in %s%s line %d: Must have exactly one comma delimiter between .struct directive operands\n",
                       filename, filename_suffix, line_num);
                success = false;
            }

            if (line[string_start_idx] != QUOTATION_CHAR) {
                printf("Error in %s%s line %d: .string directive operand must start with a \" character\n", filename,
                       filename_suffix, line_num);
                success = false;
            }
            string_end_idx = strFindNextChar(line, string_start_idx + 1, QUOTATION_CHAR);
            if (string_end_idx == -1) {
                printf("Error in %s%s line %d: .string directive operand must end with a \" character\n", filename,
                       filename_suffix, line_num);
                success = false;
            }
            free(token);
            token = strndup(line + string_start_idx, string_end_idx - string_start_idx + 1);
            if (!token) {
                memoryAllocationError();
            }
            listAppend(tokens, token);
            listAppend(operands, token);

            prev_token_stop_idx = string_end_idx + 1;

        } else if (strcmp(mnemonic, DIRECTIVE_ENTRY) == 0) {
            prev_token_stop_idx = next_token_stop_idx;
            nextOperandInLine(line, &next_token_start_idx, &next_token_stop_idx);
            if (next_token_start_idx != -1) {
                printf("Error in %s%s line %d: Directive .entry must have exactly one argument\n", filename,
                       filename_suffix, line_num);
                success = false;
            }
        } else { /* .extern */
            prev_token_stop_idx = next_token_stop_idx;
            nextOperandInLine(line, &next_token_start_idx, &next_token_stop_idx);
            if (next_token_start_idx != -1) {
                printf("Error in %s%s line %d: Directive .extern must have exactly one argument\n", filename,
                       filename_suffix, line_num);
                success = false;
            }
        }

        if (strCountCharInRange(line, OPERANDS_DELIM_CHAR, prev_token_stop_idx, strlen(line)) > 0) {
            printf("Error in %s%s line %d: Misplaced comma delimiter after last operand\n", filename, filename_suffix,
                   line_num);
            success = false;
        }
    } else if (check_non_macro_syntax) { /* instruction */
        if (is0OperandInstruction(token)) {
            nextOperandInLine(line, &next_token_start_idx, &next_token_stop_idx);
            if (next_token_start_idx != -1) {
                printf("Error in %s%s line %d: Instruction %s must have no operands\n", filename, filename_suffix,
                       line_num, token);
                success = false;
            }
        } else if (is1OperandInstruction(token)) {
            prev_token_stop_idx = next_token_stop_idx;
            nextOperandInLine(line, &next_token_start_idx, &next_token_stop_idx);
            if (next_token_start_idx == -1) {
                printf("Error in %s%s line %d: Instruction %s must have exactly one operand\n", filename,
                       filename_suffix,
                       line_num, token);
                success = false;
            } else {
                if (strCountCharInRange(line, OPERANDS_DELIM_CHAR, prev_token_stop_idx, next_token_start_idx) > 0) {
                    printf("Error in %s%s line %d: Misplaced comma delimiter before 1st operand\n",
                           filename, filename_suffix, line_num);
                    success = false;
                }
                if (success) {
                    free(token);
                    token = strndup(line + next_token_start_idx, next_token_stop_idx - next_token_start_idx);
                    if (!token) {
                        memoryAllocationError();
                    }
                    listAppend(tokens, token);
                    listAppend(operands, token);

                    prev_token_stop_idx = next_token_stop_idx;
                    nextOperandInLine(line, &next_token_start_idx, &next_token_stop_idx);
                    if (next_token_start_idx != -1) {
                        printf("Error in %s%s line %d: Instruction %s must have exactly one operand\n", filename,
                               filename_suffix, line_num, token);
                        success = false;
                    } else {
                        AddressingMode mode = getAddressingMode(token);
                        if (mode == INVALID_ADDRESSING) {
                            printf("Error in %s%s line %d: invalid operand\n", filename, filename_suffix, line_num);
                            success = false;
                        } else if (!isValidAddressing_1_OP(mnemonic, mode)) {
                            printf("Error in %s%s line %d: invalid addressing for operand and instruction\n", filename,
                                   filename_suffix, line_num);
                            success = false;
                        }
                    }
                }
            }
        } else { /* 2 operands instruction */
            char *operand_src = NULL;
            char *operand_dest = NULL;
            AddressingMode mode_src;
            AddressingMode mode_dest;

            prev_token_stop_idx = next_token_stop_idx;
            nextOperandInLine(line, &next_token_start_idx, &next_token_stop_idx);
            if (next_token_start_idx == -1) {
                printf("Error in %s%s line %d: Instruction %s must have exactly two operands\n", filename,
                       filename_suffix,
                       line_num, token);
                success = false;
            } else {
                if (strCountCharInRange(line, OPERANDS_DELIM_CHAR, prev_token_stop_idx, next_token_start_idx) > 0) {
                    printf("Error in %s%s line %d: Misplaced comma delimiter before 1st operand\n",
                           filename, filename_suffix, line_num);
                    success = false;
                }
                if (success) {
                    operand_src = strndup(line + next_token_start_idx, next_token_stop_idx - next_token_start_idx);
                    if (!operand_src) {
                        memoryAllocationError();
                    }
                    listAppend(tokens, operand_src);
                    listAppend(operands, operand_src);

                    prev_token_stop_idx = next_token_stop_idx;
                    nextOperandInLine(line, &next_token_start_idx, &next_token_stop_idx);
                    if (next_token_start_idx == -1) {
                        printf("Error in %s%s line %d: Instruction %s must have exactly two operands\n", filename,
                               filename_suffix, line_num, token);
                        success = false;
                    } else {
                        if (strCountCharInRange(line, OPERANDS_DELIM_CHAR, prev_token_stop_idx, next_token_start_idx) != 1) {
                            printf("Error in %s%s line %d: Must have exactly one comma delimiter between 1st and 2nd operands\n",
                                   filename, filename_suffix, line_num);
                            success = false;
                        }
                        operand_dest = strndup(line + next_token_start_idx, next_token_stop_idx - next_token_start_idx);
                        if (!operand_dest) {
                            memoryAllocationError();
                        }
                        listAppend(tokens, operand_dest);
                        listAppend(operands, operand_dest);

                        mode_src = getAddressingMode(operand_src);
                        mode_dest = getAddressingMode(operand_dest);

                        if (mode_src == INVALID_ADDRESSING) {
                            printf("Error in %s%s line %d: invalid source operand\n", filename, filename_suffix, line_num);
                            success = false;
                        }
                        if (mode_dest == INVALID_ADDRESSING) {
                            printf("Error in %s%s line %d: invalid destination operand\n", filename, filename_suffix, line_num);
                            success = false;
                        }
                        if (success && !isValidAddressing_2_OP(mnemonic, mode_src, mode_dest)) {
                            printf("Error in %s%s line %d: invalid addressing for operands and instruction\n", filename,
                                   filename_suffix, line_num);
                            success = false;
                        }
                    }
                }
            }
            if (operand_src) {
                free(operand_src);
            }
            if (operand_dest) {
                free(operand_dest);
            }
        }

        if (success && strCountCharInRange(line, OPERANDS_DELIM_CHAR, next_token_stop_idx, strlen(line)) > 0) {
            printf("Error in %s%s line %d: Misplaced comma delimiter after last operand\n", filename, filename_suffix,
                   line_num);
            success = false;
        }
    }

    if (success) {
        s = statementCreate(line_num, type, line, label, mnemonic, operands, tokens);
    } else {
        s = NULL;
    }
    free((char *) mnemonic);
    free((char *) label);
    free(token);
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
