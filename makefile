assembler : assembler.o common.o directiveHandlers.o instructionHandlers.o labelHandlers.o parser.o
	gcc -g -ansi -Wall -pedantic assembler.o common.o directiveHandlers.o instructionHandlers.o labelHandlers.o parser.o -o assembler -lm

assembler.o : assembler.c assembler.h
	gcc -c -ansi -Wall -pedantic assembler.c -o assembler.o

common.o : common.c common.h
	gcc -c -ansi -Wall -pedantic common.c -o common.o

directiveHandlers.o : directiveHandlers.c directiveHandlers.h
	gcc -c -ansi -Wall -pedantic directiveHandlers.c -o directiveHandlers.o -lm

instructionHandlers.o : instructionHandlers.c instructionHandlers.h 
	gcc -c -ansi -Wall -pedantic instructionHandlers.c -o instructionHandlers.o

labelHandlers.o : labelHandlers.c labelHandlers.h
	gcc -c -ansi -Wall -pedantic labelHandlers.c -o labelHandlers.o

parser.o : parser.c parser.h
	gcc -c -ansi -Wall -pedantic parser.c -o parser.o
