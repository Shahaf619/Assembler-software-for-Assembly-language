assembler: assembler.o ram.o encode.o symbols.o file.o input.o guide.o command.o
	gcc -g -Wall -ansi -pedantic assembler.o ram.o encode.o symbols.o file.o input.o guide.o command.o -o t

assembler.o: assembler.c data.h ram.h symbols.h file.h functions.h guide.h input.h
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o 

ram.o: ram.c ram.h command.h
	gcc -c -Wall -ansi -pedantic ram.c -o ram.o 

encode.o: encode.c ram.h data.h encode.h command.h input.h 
	gcc -c -Wall -ansi -pedantic encode.c -o encode.o 

symbols.o: symbols.c symbols.h input.h data.h ram.h command.h
	gcc -c -Wall -ansi -pedantic symbols.c -o symbols.o 

file.o: file.c file.h
	gcc -c -Wall -ansi -pedantic file.c -o file.o 

input.o: input.c data.h input.h guide.h
	gcc -c -Wall -ansi -pedantic input.c -o input.o 

guide.o: guide.c data.h input.h symbols.h guide.h
	gcc -c -Wall -ansi -pedantic guide.c -o guide.o

command.o: command.c data.h input.h symbols.h guide.h command.h
	gcc -c -Wall -ansi -pedantic command.c -o command.o








