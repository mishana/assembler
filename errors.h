#ifndef ASSEMBLER_ERRORS_H
#define ASSEMBLER_ERRORS_H


void memoryAllocationError(void);
void fileNotFoundError(const char *filename);
void errorWithMsg(const char *msg);

#endif
