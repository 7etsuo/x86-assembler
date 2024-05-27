; ***********************************************************
; ~isTofu
; tofu@rootstorm.com
; https://www.rootstorm.com
; ***********************************************************  

global _start   ; global is used to export the _start label.
                ; This will be the entry point to the program

section .text
_start:

; ########################################################
; %eax	Name	    %ebx	        %ecx	        %edx
; 4	    sys_write	unsigned int	const char *	size_t
; syscall - write(1, msg, len);
; ########################################################

    mov eax, 4
    mov ebx, 1
    mov ecx, msg
    mov edx, len
    int 0x80

; ########################################################
; syscall - exit(0);
; ########################################################

    mov eax, 1
    mov ebx, 0
    int 0x80

; The data segment of our program.
section .data
    msg: db "Hello, World!",0xa
    len: equ $-msg
