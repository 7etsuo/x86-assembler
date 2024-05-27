; ***********************************************************
; ~isTofu
; tofu@rootstorm.com
; https://www.rootstorm.com
; ***********************************************************  

[BITS 32]
global _start
_start:
incbin 'test.o'
	mov eax, 1
	int 80h