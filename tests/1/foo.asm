section .data
	hello:     db 'Hello world!', 0xA
	helloLen:  equ $ - hello

section .text
	global foo

foo:
	mov rax, 1
	mov rdi, 1
	mov rsi, hello
	mov rdx, helloLen
	syscall

	mov rax, 0x3C
	mov rdi, 42
	syscall
