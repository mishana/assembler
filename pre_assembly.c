//
// Created by misha on 27/07/2022.
//
#define _GNU_SOURCE

#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "pre_assembly.h"
#include "linkedlist.h"
#include "errors.h"
#include "macro.h"
#include "parser.h"
#include "file_utils.h"


#define SOURCE_FILE_SUFFIX ".as"
#define AFTER_MACRO_SUFFIX ".am"

#define MAX_LINE_LEN 2048


/**
 * It takes a source file and a destination file and copies the source file to the destination file, but it also replaces
 * any macros with their definitions.
 *
 * @param src_file The file to read from.
 * @param dst_file The file to write the output to.
 * @return true if the operation was successful, false otherwise.
 */
bool unfold_macros(FILE *src_file, FILE *dst_file) {
    List macros = listCreate((list_eq) macroCmp, (list_copy) macroCopy, (list_free) macroDestroy);

    bool success = true;

    bool is_macro = false;
    const char *macro_name;
    char *macro_body = NULL;
    int macro_def_line_num;

    int line_num = 0;
    char line[MAX_LINE_LEN];
    while (fgets(line, MAX_LINE_LEN, src_file) != NULL) {
        /* It's parsing the line. */
        line_num++;
        Statement s = parse(line, line_num);
        if (!s) { // comment or empty line
            continue;
        }

        if (statementGetType(s) == MACRO_START) {
            is_macro = true;
            macro_name = strdup(listGetDataAt(statementGetOperands(s), 0));
            statementCheckSyntax(s);
            macro_def_line_num = line_num;
            // TODO: check validity of macro name and validity of line

        } else if (statementGetType(s) == MACRO_END) {
            statementCheckSyntax(s);
            Macro m = macroCreate(macro_name, macro_body, macro_def_line_num);

            Macro found_macro;
            ListResult res = listFind(macros, m, (void **) &found_macro);

            if (res == LIST_NOT_FOUND) {  // macro not found
                listAppend(macros, m);
            } else { // found macro
                printf("Macro %s on line %d was already previously defined on line %d\n", macro_name,
                       macro_def_line_num, macroGetDefLineNum(found_macro));
                success = false;
                macroDestroy(found_macro);
            }

            macroDestroy(m);

            is_macro = false;
            free((void *) macro_name);
            free(macro_body);
            macro_body = NULL;

        } else if (is_macro) { // inside macro - append to macro body
            char *tmp = macro_body;

            if (!macro_body) {
                /* It's appending the line to the macro body. */
                if (asprintf(&macro_body, "%s", line) == -1)
                    memoryAllocationError();
            } else {
                /* It's appending the line to the macro body. */
                if (asprintf(&macro_body, "%s%s", macro_body, line) == -1)
                    memoryAllocationError();
                free(tmp);
            }

        } else { // outside macro definition, check if referencing macro that needs unfolding
            if (statementGetType(s) == COMMENT || statementGetType(s) == EMPTY_LINE) {
                fputs(line, dst_file);
                statementDestroy(s);
                continue;
            }
            const char *first_word = listGetDataAt(statementGetTokens(s), 0);

            /* It's creating a dummy macro with the name of the first word in the line. */
            Macro dummy = macroCreate(first_word, NULL, -1);
            Macro found_macro;
            ListResult res = listFind(macros, dummy, (void **) &found_macro);
            macroDestroy(dummy);

            if (res == LIST_FOUND) {
                fputs(macroGetBody(found_macro), dst_file);
                macroDestroy(found_macro);
            } else {
                fputs(line, dst_file);
            }
        }
        statementDestroy(s);
    }
    listDestroy(macros);

    return success;
}

/**
 * It runs the pre-assembly step of the pipeline.
 *
 * @param filename The name of the file to be assembled.
 * @return Whether the pre-assembly step was successful.
 */
bool run_pre_assembly(const char *filename) {
    FILE *src_file = openFileWithSuffix(filename, "r", SOURCE_FILE_SUFFIX);
    FILE *dst_file = openFileWithSuffix(filename, "w", AFTER_MACRO_SUFFIX);

    bool res = unfold_macros(src_file, dst_file);

    fclose(src_file);
    fclose(dst_file);

    return res;
}
