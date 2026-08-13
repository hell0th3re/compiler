section .bss
    thename: resq 5
section .text
global _start
_start:
    mov qword [thename + 0], 97
    mov qword [thename + 8], 98
    mov qword [thename + 16], 99
    mov qword [thename + 24], 100
    mov qword [thename + 32], 101
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
