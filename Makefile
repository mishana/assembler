CC=gcc
OBJS=base_conversion.o const_tables.o errors.o file_utils.o first_pass.o linkedlist.o machine_code.o macro.o main.o memory_code.o parser.o pre_assembly.o rules.o second_pass.o str_utils.o symtab.o types_utils.o
EXEC=assembler
CFLAGS=-Wall -ansi -pedantic
LIBS=-lm

$(EXEC) : $(OBJS)
	$(CC) $(OBJS) -o $@ $(LIBS) $(CFLAGS)

base_conversion.o: base_conversion.c base_conversion.h
const_tables.o: const_tables.c const_tables.h linkedlist.h str_utils.h types_utils.h
errors.o: errors.c errors.h
file_utils.o: file_utils.c file_utils.h str_utils.h errors.h
first_pass.o: first_pass.c first_pass.h linkedlist.h parser.h str_utils.h symtab.h const_tables.h file_utils.h memory_code.h machine_code.h
linkedlist.o: linkedlist.c linkedlist.h errors.h
machine_code.o: machine_code.c machine_code.h parser.h linkedlist.h errors.h const_tables.h str_utils.h symtab.h base_conversion.h
macro.o: macro.c macro.h errors.h
main.o: main.c pre_assembly.h first_pass.h second_pass.h file_utils.h errors.h
memory_code.o: memory_code.c memory_code.h parser.h const_tables.h errors.h base_conversion.h
parser.o: parser.c parser.h linkedlist.h str_utils.h errors.h const_tables.h rules.h types_utils.h
pre_assembly.o: pre_assembly.c pre_assembly.h linkedlist.h errors.h macro.h parser.h file_utils.h
rules.o: rules.c rules.h linkedlist.h const_tables.h
second_pass.o: second_pass.c second_pass.h linkedlist.h file_utils.h machine_code.h memory_code.h const_tables.h symtab.h base_conversion.h
str_utils.o: str_utils.c str_utils.h linkedlist.h errors.h
symtab.o: symtab.c symtab.h linkedlist.h errors.h
types_utils.o: types_utils.c types_utils.h

clean:
	rm -f $(OBJS) $(EXEC)
