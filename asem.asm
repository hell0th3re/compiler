global _start
_start:
    push 4
    push 108
    push 1
    push 2
    push 3
    mov rax, [rsp + 24]
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
    mov rax, [rsp + 32]
    push rax
    pop rdi
    mov rax, 60
    syscall
