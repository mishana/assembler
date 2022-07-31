cmake_minimum_required(VERSION 3.22)
project(assembler C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -ansi -pedantic")

add_executable(assembler main.c first_pass.c first_pass.h second_pass.c second_pass.h base_conversion.c base_conversion.h symtab.c symtab.h memmap.c memmap.h const_tables.c const_tables.h pre_assembly.c pre_assembly.h errors.h linkedlist.c linkedlist.h parser.c parser.h str_utils.c str_utils.h macro.c macro.h errors.c)
