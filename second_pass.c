//
// Created by misha on 27/07/2022.
//

#include <stdio.h>
#include "second_pass.h"
#include "linkedlist.h"
#include "file_utils.h"
#include "machine_code.h"
#include "memmap.h"

#define SOURCE_FILE_SUFFIX ".am"
#define OBJECT_FILE_SUFFIX ".ob"
#define ENTRIES_FILE_SUFFIX ".ent"
#define EXTERNAL_FILE_SUFFIX ".ext"

#define MAX_LINE_LEN 2048


static void writeCodeToObjectFile(List machine_codes, List memory_codes, FILE *obj_file) {
    int i = 0;
    for (; i < listLength(machine_codes); i++) {
        MachineCode mc = (MachineCode)listGetDataAt(machine_codes, i);
        machineCodeToObjFile(mc, obj_file);
    }
    for (i = 0; i < listLength(memory_codes); i++) {
        MemoryCode mc = (MemoryCode)listGetDataAt(memory_codes, i);
        memoryCodeToObjFile(mc, obj_file);
    }
}

static void updateAdressesFromSymtab(List machine_codes, List symtab, const char *filename) {
    for (int i = 0; i < listLength(machine_codes); ++i) {
        MachineCode mc = listGetDataAt(machine_codes, i);
        machineCodeUpdateFromSymtab(mc, symtab, SOURCE_FILE_SUFFIX, filename);
    }
}

void run_second_pass(const char *filename, List symtab, List machine_codes, List memory_codes) {
    FILE *src_file = openFileWithSuffix(filename, "r", SOURCE_FILE_SUFFIX);

    FILE *entries_file = openFileWithSuffix(filename, "w", ENTRIES_FILE_SUFFIX);
    FILE *external_file = openFileWithSuffix(filename, "w", EXTERNAL_FILE_SUFFIX);
    FILE *object_file = openFileWithSuffix(filename, "w", OBJECT_FILE_SUFFIX);

    updateAdressesFromSymtab(machine_codes, symtab, filename);
    writeCodeToObjectFile(machine_codes, memory_codes, object_file);
    // TODO: write to entries and external files

    fclose(src_file);
    fclose(entries_file);
    fclose(external_file);
    fclose(object_file);

    listDestroy(symtab);
    listDestroy(machine_codes);
    listDestroy(memory_codes);
}
