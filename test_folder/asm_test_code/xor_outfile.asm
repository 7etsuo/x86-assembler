; nasm -f elf -o outfile.o outfile.asm
; ld -m elf_i386 outfile.o -o a.out 

[BITS 32]                                          
global _start                                      
_start:                                            
db 0x33,0x0a                            ;  xor ECX, dword [EDX]
db 0x33,0x0c,0x32                       ;  xor ECX, dword [EDX + ESI]
db 0x33,0x4c,0x72,0x08                  ;  xor ECX, dword [EDX + ESI*2 + 8]
db 0x31,0x0a                            ;  xor dword [EDX], ECX
db 0x31,0x0c,0x32                       ;  xor dword [EDX + ESI], ECX
db 0x31,0x4c,0x72,0x08                  ;  xor dword [EDX + ESI*2 + 8], ECX
db 0x31,0xc0                            ;  xor eax, eax
db 0x81,0x30,0xcd,0xab,0xcd,0xab        ;  xor dword [eax], 0xabcdabcd
db 0x34,0x45                            ;  xor al, 0x12345
db 0x35,0xef,0xcd,0xab,0x89             ;  xor ax, 0x123456789abcdef
db 0x35,0xef,0xcd,0xab,0x89             ;  xor eax, 0x123456789abcdef