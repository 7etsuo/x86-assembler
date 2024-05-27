xor ECX, dword [EDX]
xor ECX, dword [EDX + ESI]
xor ECX, dword [EDX + ESI*2 + 8]
xor dword [EDX], ECX
xor dword [EDX + ESI], ECX
xor dword [EDX + ESI*2 + 8], ECX

xor eax, eax
xor dword [eax], 0xabcdabcd 

xor al, 0x12345
xor ax, 0x123456789abcdef
xor eax, 0x123456789abcdef
