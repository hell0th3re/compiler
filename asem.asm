global _start
_start:
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
    push 32
    mov rsi, rsp
    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    syscall
    pop rax
    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    push 115
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
    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    syscall
    pop rax
    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    push 97
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
    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    syscall
    pop rax
    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    push 5
    pop rdi
    mov rax, 60
    syscall
