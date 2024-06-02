all:
	../../build/cecler -c main.clr -o main.o
	nasm -f elf64 foo.asm
	nasm -f elf64 stdlib.asm
	ld main.o foo.o stdlib.o -o program