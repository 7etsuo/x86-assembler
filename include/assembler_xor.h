/** x86 assembler
 * https://www.x.com/7etsuo
 */

#ifndef ASSEMBLER_XOR_H
#define ASSEMBLER_XOR_H

#include <stdint.h>

 /* ******************* XOR—Logical Exclusive OR
 * Opcode
 * =============================================================================================
 * 34            ib  XOR     AL,     imm8    I   Valid Valid  AL     XOR imm8.
 * 35            iw  XOR     AX,     imm16   I   Valid Valid  AX     XOR imm16.
 * 35            id  XOR     EAX,    imm32   I   Valid Valid  EAX    XOR imm32.
 * REX.W + 35    id  XOR     RAX,    imm32   I   Valid N.E.   RAX    XOR imm32   (sign-extended).
 * 80 /6         ib  XOR     r/m8,   imm8    MI  Valid Valid  r/m8   XOR imm8.
 * REX + 80 /6   ib  XOR     r/m8*,  imm8    MI  Valid N.E.   r/m8   XOR imm8.
 * 81 /6         iw  XOR     r/m16,  imm16   MI  Valid Valid  r/m16  XOR imm16.
 * 81 /6         id  XOR     r/m32,  imm32   MI  Valid Valid  r/m32  XOR imm32.
 * REX.W + 81 /6 id  XOR     r/m64,  imm32   MI  Valid N.E.   r/m64  XOR imm32   (sign-extended).
 * 83 /6         ib  XOR     r/m16,  imm8    MI  Valid Valid  r/m16  XOR imm8    (sign-extended).
 * 83 /6         ib  XOR     r/m32,  imm8    MI  Valid Valid  r/m32  XOR imm8    (sign-extended).
 * REX.W + 83 /6 ib  XOR     r/m64,  imm8    MI  Valid N.E.   r/m64  XOR imm8    (sign-extended).
 * 30 /r             XOR     r/m8,   r8      MR  Valid Valid  r/m8   XOR r8.
 * REX + 30 /r       XOR     r/m8*,  r8*     MR  Valid N.E.   r/m8   XOR r8.
 * 31 /r             XOR     r/m16,  r16     MR  Valid Valid  r/m16  XOR r16.
 * 31 /r             XOR     r/m32,  r32     MR  Valid Valid  r/m32  XOR r32.
 * REX.W + 31 /r     XOR     r/m     64,r64  MR  Valid N.E.   r/m64  XOR r64.
 * 32 /r             XOR     r8,     r/m8    RM  Valid Valid  r8     XOR r/m8.
 * REX + 32 /r       XOR     r8*,    r/m8*   RM  Valid N.E.   r8     XOR r/m8.
 * 33 /r             XOR     r16,    r/m16   RM  Valid Valid  r16    XOR r/m16.
 * 33 /r             XOR     r32,    r/m32   RM  Valid Valid  r32    XOR r/m32.
 * REX.W + 33 /r     XOR     r64,    r/m64   RM  Valid N.E.   r64    XOR r/m64.
 * =============================================================================================
 */

 // register, immediate
#define XOR_I_AL_IMM        0x34
#define XOR_I_AX_IMM        0x35
#define XOR_I_EAX_IMM       0x35

// memory, immediate
#define XOR_MI_RM8_IMM8     0x80
#define XOR_MI_RM16_IMM16   0x81
#define XOR_MI_RM32_IMM32   0x81

#define XOR_MI_RM16_IMM8    0x83
#define XOR_MI_RM32_IMM8    0x83

// register, memory
#define XOR_RM_R8_RM8       0x32 
#define XOR_RM_R16_RM16     0x33
#define XOR_RM_R32_RM32     0x33

// memory, register
#define XOR_MR_RM8_R8       0x30
#define XOR_MR_RM16_R16     0x31
#define XOR_MR_RM32_R32     0x31



struct OpData; // forward declaration

int16_t
get_xor_immediate_value_and_opcode(struct OpData* operands, const char* arg1, const char* arg2);


#endif // ASSEMBLER_H
