section .bss
    thename: resq 5
section .text
global _start
_start:
    mov qword [thename + 0], 5
    mov qword [thename + 8], 4
    mov qword [thename + 16], 97
    mov qword [thename + 24], 2
    mov qword [thename + 32], 1
    mov rax, thename
    push rax
    push 97
    mov rax, [thename + 16]
    push rax
    mov rsi, rsp
    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    syscall
    pop rax
    push 10
    mov rsi, rsp
    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    syscall
    pop rax
    push 2
    pop rdi
    mov rax, 60
    syscall
