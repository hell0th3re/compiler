section .bss
section .text
global _start
_start:
    mov rax, 90
    add rax, 9
    push rax
    mov rax, [rsp + 0]
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
