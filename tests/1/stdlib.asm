section .text
	global fork
    global mmap

fork:
	mov rax, 0x39
	syscall
    ret

mmap:
    mov rax, 0x09
    mov rsi, rdi
    mov rdi, 0
    mov rdx, 3
    mov r10, 34
    mov r8, -1
    mov r9, 0
    syscall
    ret
