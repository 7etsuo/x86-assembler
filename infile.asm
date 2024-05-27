l1:
	mov    eax, dword [EDX]
	jmp l2
   	mov    eax, dword [EDX]
	nop
l2:
	nop
    xor eax, eax
    nop
	jmp l1
	nop
    mov eax, 0x1
    mov ebx, 0x0
    int 0x80
    nop