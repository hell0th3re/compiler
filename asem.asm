global _start
_start:
    push 21
    push 99
    push 102
    push 3
    mov rax, [rsp + 16]
    push rax
    mov rsi, rsp
    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    syscall
    pop rax
    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    push 10
    mov rsi, rsp
    syscall
    pop rax
    mov rax, [rsp + 8]
    push rax
    mov rsi, rsp
    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    syscall
    pop rax
    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    push 10
    mov rsi, rsp
    syscall
    pop rax
    push 116
    mov rsi, rsp
    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    syscall
    pop rax
    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    push 10
    mov rsi, rsp
    syscall
    pop rax
    mov rax, [rsp + 24]
    push rax
    pop rdi
    mov rax, 60
    syscall
