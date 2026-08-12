global _start
_start:
    push 92
    push 99
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
    mov rdi, 21
    mov rax, 60
    syscall
