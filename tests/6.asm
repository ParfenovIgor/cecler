section .data
	hello:     db 'Hello world!', 0xA
	helloLen:  equ $ - hello

section .text
	global foo 

foo:
	mov eax, 4
	mov ebx, 1
	mov ecx, hello
	mov edx, helloLen
	int 0x80

	mov eax, 1
	mov ebx, 0
	int 0x80
