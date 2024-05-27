; ***********************************************************
; ~isTofu
; tofu@rootstorm.com
; https://www.rootstorm.com
; ***********************************************************  

global _start
section .text
_start:
  mov rax, 1		; write(fd, "Hello, world!\n", sizeof("Hello, world!\n"); 
  mov rdi, 1
  mov rsi, msg
  mov rdx, msglen
  syscall

  mov rax, 60		; exit(0)
  mov rdi, 0
  syscall

  section .rodata
  msg: db "Hello, world!", 10
  msglen: equ $ - msg