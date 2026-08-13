global _start
_start:
    push 100
    push 99
    push 3
    mov rax, [rsp + 8]
    push rax
    mov rsi, rsp
    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    syscall
    push 10
    mov rsi, rsp
    syscall
    pop rax
    mov rax, [rsp + 24]
    push rax
    mov rsi, rsp
    mov rax, 1
    mov rdi, 1
    mov rdx, 1
    syscall
    push 10
    mov rsi, rsp
    syscall
    pop rax
    mov rax, [rsp + 8]
    push rax
    pop rdi
    mov rax, 60
    syscall
