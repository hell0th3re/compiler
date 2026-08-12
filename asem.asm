global _start
_start:
    push 5
    push 99
    push 115
    mov rsi, rsp
    pop rax
    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    syscall
    push 10
    mov rsi, rsp
    syscall
    mov rax, [rsp + 16]
    push rax
    pop rdi
    mov rax, 60
    syscall
