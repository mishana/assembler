cmake_minimum_required(VERSION 3.22)
project(assembler C)

set(CMAKE_C_STANDARD 99)

add_executable(assembler main.c first_pass.c first_pass.h second_pass.c second_pass.h utils.c utils.h symtab.c symtab.h memmap.c memmap.h const_tables.c const_tables.h pre_assembly.c pre_assembly.h errors.h)
