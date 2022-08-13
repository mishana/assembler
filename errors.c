//
// Created by misha on 30/07/2022.
//

#include "errors.h"
#include <stdio.h>
#include <stdlib.h>

#define MEMORY_ALLOCATION_ERROR -2
#define FILE_NOT_FOUND_ERROR -3


void memoryAllocationError(void) {
    printf("Memory Allocation ERROR :(");
    exit(MEMORY_ALLOCATION_ERROR);
}

void fileNotFoundError(const char *filename) {
    printf("File %s not found\n", filename);
    exit(FILE_NOT_FOUND_ERROR);
}

void errorWithMsg(const char *msg) {
    printf("%s", msg);
    exit(-1);
}
