#include <stdio.h> 
#include <stdlib.h> 

const char* reg_32[] = { "eax", "ecx", "edx", "ebx", "ebp", "esi", "edi", NULL };
int main()
{
    for (int j = 1; j < 16; j = j * 2) {
        for (int i = 0x1; i <= 0xff; i++) {
            for (int k = 0, l = 6; k < l; k++, l--) {
                printf("push dword [%s + %s*%d + 0x%x]\n", reg_32[k], reg_32[l], j, i);
            }
        }
    }
}