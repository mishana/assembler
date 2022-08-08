//
// Created by misha on 27/07/2022.
//
#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "pre_assembly.h"
#include "linkedlist.h"
#include "errors.h"
#include "macro.h"
#include "parser.h"


#define SOURCE_FILE_SUFFIX ".as"
#define AFTER_MACRO_SUFFIX ".am"

#define MAX_FILENAME_LEN 2048
#define MAX_LINE_LEN 2048


void concat_filename_and_suffix(char *filename_with_suffix, const char *filename, const char *suffix) {
    size_t filename_with_suffix_len = strlen(filename) + strlen(suffix);
    /* Concatenating the filename with the suffix. */
    snprintf(filename_with_suffix, filename_with_suffix_len + 1, "%s%s", filename, suffix);
}

void unfold_macros(FILE *src_file, FILE *dst_file) {
    List macros = listCreate((list_eq) macroCmp, (list_copy) macroCopy, (list_free) macroDestroy);

    bool is_macro = false;
    const char *macro_name;
    char *macro_body = NULL;

    char line[MAX_LINE_LEN];
    while (fgets(line, MAX_LINE_LEN, src_file) != NULL) {
        /* It's parsing the line. */
        Statement s = parse(line);
        if (!s) { // comment or empty line
            continue;
        }

        if (statementGetType(s) == MACRO_START) {
            is_macro = true;
            macro_name = nodeGetData(listHead(statementGetOperands(s)));

        } else if (statementGetType(s) == MACRO_END) {
            Macro m = macroCreate(macro_name, macro_body);
            listAppend(macros, m);
            macroDestroy(m);

            is_macro = false;
            free(macro_body);

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
            const char *first_word = nodeGetData(listHead(statementGetTokens(s)));

            /* It's creating a dummy macro with the name of the first word in the line. */
            Macro dummy = macroCreate(first_word, NULL);
            Macro found_macro;
            ListResult res = listFind(macros, dummy, (void **) &found_macro);
            macroDestroy(dummy);

            if (res == LIST_FOUND) {
                fputs(macroGetBody(found_macro), dst_file);
                macroDestroy(found_macro);
            }
            else {
                fputs(line, dst_file);
            }
        }
        statementDestroy(s);
    }
    listDestroy(macros);
}

void run_pre_assembly(const char *filename) {
    char filename_with_suffix[MAX_FILENAME_LEN];

    concat_filename_and_suffix(filename_with_suffix, filename, SOURCE_FILE_SUFFIX);
    FILE *src_file = fopen(filename_with_suffix, "r");

    concat_filename_and_suffix(filename_with_suffix, filename, AFTER_MACRO_SUFFIX);
    FILE *dst_file = fopen(filename_with_suffix, "w");

    unfold_macros(src_file, dst_file);

    fclose(src_file);
    fclose(dst_file);
}