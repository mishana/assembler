//
// Created by misha on 30/07/2022.
//

#include "parser.h"
#include "str_utils.h"
#include "errors.h"
#include <stdlib.h>
#include <string.h>
#include "const_tables.h"

#define WHITESPACE_DELIM " \t\n "
#define OPERANDS_DELIM ","
#define LABEL_SUFFUX ":"
#define COMMENT_PREFIX ";"

#define START_MACRO_STR "macro"
#define END_MACRO_STR "endmacro"


struct statement_t {
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
    // TODO: check label validity
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
Statement parse(const char *line) {
    if (isCommentLine(line)) {
        return NULL;
    }

    // TODO: check validity of operands delimiters, amount and placement
    const char *line_replaced = strReplace(line, OPERANDS_DELIM, WHITESPACE_DELIM);
    if (!line_replaced) {
        memoryAllocationError();
    }

    List tokens = strSplit(line_replaced, WHITESPACE_DELIM);
    /* Checking that the list of tokens is not empty. */
    if (listLength(tokens) == 0) { // empty line
        listDestroy(tokens);
        free((char *) line_replaced);
        return NULL;
    }

    free((char *) line_replaced);

    int token_index = 0;
    const char *token = listGetDataAt(tokens, token_index);

    const char *label = NULL;
    if (isLabel(token)) {
        label = token;
        // TODO: check label validity
        token_index++;
        token = listGetDataAt(tokens, token_index);
    }
    StatementType type;
    if (isMacroStart(token) && listLength(tokens) == 2) {
        type = MACRO_START;

        const char *macro_name = listGetDataAt(tokens, 1);
        if (isDirective(macro_name) || isInstruction(macro_name)) {
            // FIXME: error or something
            listDestroy(tokens);
            return NULL;
        }
    } else if (isMacroEnd(token) && listLength(tokens) == 1) {
        type = MACRO_END;
    } else if (isDirective(token)) {
        type = DIRECTIVE;
    } else if (isInstruction(token)) {
        type = INSTRUCTION;
    } else {
        // TODO: exception or something?
        type = OTHER;
    }

    const char *mnemonic = token;

    token_index++;
    List operands = listCopyFromIndex(tokens, token_index);

    Statement s = statementCreate(type, line, label, mnemonic, operands, tokens);
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
statementCreate(StatementType type, const char *raw_text, const char *label, const char *mnemonic, List operands,
                List tokens) {
    Statement s = (Statement) malloc(sizeof(*s));
    if (!s)
        memoryAllocationError();

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

