#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "errors.h"
#include "pre_assembly.h"
#include "first_pass.h"
#include "second_pass.h"
#include "file_utils.h"


int main(int argc, char **argv) {
    if (argc < 2) {
        errorWithMsg("Not enough arguments! Need to specify files to compile (without suffix).");
    }

    for (int i = 1; i < argc; ++i) {
        printf("============================================================================================\n");

        const char *file_to_compile = argv[i];

        printf("1. Run pre-assembly for %s\n", file_to_compile);
        bool pre_assembly_res = run_pre_assembly(file_to_compile);
        if (!pre_assembly_res) {
            printf("Pre-assembly for %s failed. cleaning up and skipping first-pass", file_to_compile);
            removeFileWithSuffix(file_to_compile, AFTER_MACRO_SUFFIX);
            continue;
        } else {
            printf("Pre-assembly for %s succeeded. %s%s file created\n", file_to_compile, file_to_compile, AFTER_MACRO_SUFFIX);
        }

        printf("2. Run first-pass for %s\n", file_to_compile);
        List symtab, machine_codes, memory_codes;
        bool first_pass_res = run_first_pass(file_to_compile, &symtab, &machine_codes, &memory_codes);
        if (!first_pass_res) {
            printf("First-pass for %s failed. skipping second-pass\n", file_to_compile);
            listDestroy(symtab);
            listDestroy(machine_codes);
            listDestroy(memory_codes);
            continue;
        }

        printf("3. Run second-pass for %s\n", file_to_compile);
        bool second_pass_res = run_second_pass(file_to_compile, symtab, machine_codes, memory_codes);
        if (!second_pass_res) {
            printf("Second-pass for %s failed. cleaning up artifacts..\n", file_to_compile);
            removeFileWithSuffix(file_to_compile, OBJECT_FILE_SUFFIX);
            removeFileWithSuffix(file_to_compile, ENTRIES_FILE_SUFFIX);
            removeFileWithSuffix(file_to_compile, EXTERNAL_FILE_SUFFIX);
        } else {
            printf("Second-pass for %s succeeded. %s%s file created\n", file_to_compile, file_to_compile, OBJECT_FILE_SUFFIX);
        }
    }

    return 0;
}
