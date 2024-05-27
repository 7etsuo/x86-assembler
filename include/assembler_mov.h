/** x86 assembler
 * https://www.x.com/7etsuo
 */

#ifndef ASSEMBLER_MOVE_H
#define ASSEMBLER_MOVE_H

/*
  * ========================================================================================================================================================================
  * OP | REX.W + OP | register operand            |         Instruction Column    | OP  | 64Bit | Comp/Leg | Description
  *    |            | and an r/m operand.         |                               | /EN | Mode  | Mode     |
  * ************************************************************************************************************************************************************************
  * 88 |            | /r                          |     MOV r/m8,        r8       | MR  | Valid | Valid    | Move r8                                     to r/m8.
  *    | REX.W + 88 | /r                          |     MOV r/m8 ***,    r8 ***   | MR  | Valid | N.E.     | Move r8                                     to r/m8.
  * 89 |            | /r                          |     MOV r/m16,       r16      | MR  | Valid | Valid    | Move r16                                    to r/m16.
  * 89 |            | /r                          |     MOV r/m32,       r32      | MR  | Valid | Valid    | Move r32                                    to r/m32.
  *    | REX.W + 89 | /r                          |     MOV r/m64,       r64      | MR  | Valid | N.E.     | Move r64                                    to r/m64.
  * ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  * 8A |            | /r                          |     MOV r8,          r/m8     | RM  | Valid | Valid    | Move r/m8                                   to r8.
  *    | REX.W + 8A | /r                          |     MOV r8***,       r/m8***  | RM  | Valid | N.E.     | Move r/m8                                   to r8.
  * 8B |            | /r                          |     MOV r16,         r/m16    | RM  | Valid | Valid    | Move r/m16                                  to r16.
  * 8B |            | /r                          |     MOV r32,         r/m32    | RM  | Valid | Valid    | Move r/m32                                  to r32.
  *    | REX.W + 8B | /r                          |     MOV r64,         r/m64    | RM  | Valid | N.E.     | Move r/m64                                  to r64.
  * ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  * 8C |            | /r                          |     MOV r/m16,       Sreg**   | MR  | Valid | Valid    | Move segment register                       to r/m16.
  * 8C |            | /r                          |     MOV r16/r32/m16, Sreg**   | MR  | Valid | Valid    | Move zero extended 16-bit segment register  to r16/r32/m16.
  *    | REX.W + 8C | /r                          |     MOV r64/m16,     Sreg**   | MR  | Valid | Valid    | Move zero extended 16-bit segment register  to r64/m16.
  * ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  * 8E |            | /r                          |     MOV Sreg,        r/m16**  | RM  | Valid | Valid    | Move r/m16                                  to segment register.
  *    | REX.W + 8E | /r                          |     MOV Sreg,        r/m64**  | RM  | Valid | Valid    | Move lower 16 bits of r/m64                 to segment register.
  * ************************************************************************************************************************************************************************
  * A0 |            |                             |     MOV AL,          moffs8*  | FD  | Valid | Valid    | Move byte at (seg:offset)                   to AL.
  *    | REX.W + A0 |                             |     MOV AL,          moffs8*  | FD  | Valid | N.E.     | Move byte at (offset)                       to AL.
  * A1 |            |                             |     MOV AX,          moffs16* | FD  | Valid | Valid    | Move word at (seg:offset)                   to AX.
  * A1 |            |                             |     MOV EAX,         moffs32* | FD  | Valid | Valid    | Move doubleword at (seg:offset)             to EAX.
  *    | REX.W + A1 |                             |     MOV RAX,         moffs64* | FD  | Valid | N.E.     | Move quadword at (offset)                   to RAX.
  * ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  * A2 |            |                             |     MOV moffs8,      AL       | TD  | Valid | Valid    | Move AL                                     to (seg:offset).
  *    | REX.W + A2 |                             |     MOV moffs8 *** , AL       | TD  | Valid | N.E.     | Move AL                                     to (offset).
  * A3 |            |                             |     MOV moffs16*,    AX       | TD  | Valid | Valid    | Move AX                                     to (seg:offset).
  * A3 |            |                             |     MOV moffs32*,    EAX      | TD  | Valid | Valid    | Move EAX                                    to (seg:offset).
  *    | REX.W + A3 |                             |     MOV moffs64*,    RAX      | TD  | Valid | N.E.     | Move RAX                                    to (offset).
  * ========================================================================================================================================================================
  * OP | REX.W + OP | +rb | /DIGIT  | 1-byte (ib) |         Instruction Column    | OP  | 64Bit | Comp/Leg | Description
  *    |            | +rw |         | 2-byte (iw) |                               | /EN | Mode  | Mode     |
  *    |            | +rd |         | 4-byte (id) |                               |     |       |          |
  *    |            | +ro |         | 8-byte (io) |                               |     |       |          |
  * ************************************************************************************************************************************************************************
  * B0 |            | +rb |         |    ib       |    MOV r8,           imm8     | OI  | Valid | Valid    | Move imm8                                   to r8.
  *    | REX.W + B0 | +rb |         |    ib       |    MOV r8 *** ,      imm8     | OI  | Valid | N.E.     | Move imm8                                   to r8.
  * B8 |            | +rw |         |    iw       |    MOV r16,          imm16    | OI  | Valid | Valid    | Move imm16                                  to r16.
  * B8 |            | +rd |         |    id       |    MOV r32,          imm32    | OI  | Valid | Valid    | Move imm32                                  to r32.
  *    | REX.W + B8 | +rd |         |    io       |    MOV r64,          imm64    | OI  | Valid | N.E.     | Move imm64                                  to r64.
  * ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  * C6 |            |     |   /0    |    ib       |    MOV r/m8,         imm8     | MI  | Valid | Valid    | Move imm8                                   to r/m8.
  *    | REX.W + C6 |     |   /0    |    ib       |    MOV r/m8***,      imm8     | MI  | Valid | N.E.     | Move imm8                                   to r/m8.
  * C7 |            |     |   /0    |    iw       |    MOV r/m16,        imm16    | MI  | Valid | Valid    | Move imm16                                  to r/m16.
  * C7 |            |     |   /0    |    id       |    MOV r/m32,        imm32    | MI  | Valid | Valid    | Move imm32                                  to r/m32.
  *    | REX.W + C7 |     |   /0    |    id       |    MOV r/m64,        imm32    | MI  | Valid | N.E.     | tofu@rootstorm.comMove imm32 sign extended                    to 64-bits to r/m64.
  * ========================================================================================================================================================================
  * DETAILS:
  * @ /digit — 
  * A digit between 0 and 7 indicates that the ModR/M byte of the instruction uses only the r/m (register or memory) operand.
  * The reg field contains the digit that provides an extension MOV  EAX, DWORD PTR [ESI] to the instruction's opcode.
  *
  * @ [+rb, +rw, +rd, +ro] — 
  * Indicated the lower 3 bits of the opcode byte is used to encode the register operand
  * without a modR/M byte. The instruction lists the corresponding hexadecimal value of the opcode byte with low
  * 3 bits as 000b. In non-64-bit mode, a register code, from 0 through 7, is added to the hexadecimal value of the
  * opcode byte. In 64-bit mode, indicates the four bit field of REX.b and opcode[2:0] field encodes the register
  * operand of the instruction. “+ro” is applicable only in 64-bit mode. See Table 3-1 for the codes.
  *
  *
  * @ [/r] — Indicates that the ModR/M byte of the instruction contains a register operand and an r/m operand.
  * cb, cw, cd, cp, co, ct — A 1-byte (cb), 2-byte (cw), 4-byte (cd), 6-byte (cp), 8-byte (co) or 10-byte (ct) value
  * following the opcode. This value is used to specify a code offset and possibly a new value for the code segment
  * register.
  *
  * @ [ib, iw, id, io] — A 1-byte (ib), 2-byte (iw), 4-byte (id) or 8-byte (io) immediate operand to the instruction that
  * follows the opcode, ModR/M bytes or scale-indexing bytes. The opcode determines if the operand is a signed
  * value. All words, doublewords and quadwords are given with the low-order byte first.
  *
  * @ [+rb, +rw, +rd, +ro] — Indicated the lower 3 bits of the opcode byte is used to encode the register operand
  * without a modR/M byte. The instruction lists the corresponding hexadecimal value of the opcode byte with low
  * 3 bits as 000b. In non-64-bit mode, a register code, from 0 through 7, is added to the hexadecimal value of the
  * opcode byte. In 64-bit mode, indicates the four bit field of REX.b and opcode[2:0] field encodes the register
  * operand of the instruction. “+ro” is applicable only in 64-bit mode. See Table 3-1 for the codes.
  *
  * ------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  *
  * Intel® 64 and IA-32 Architectures - Volume 2 (2A, 2B, 2C & 2D): Instruction Set Reference, A-Z
  * Reference: https://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-instruction-set-reference-manual-325383.pdf
  * PAGE: 3.1.1.3
  */
// MR 
#define MOV_RM8_R8      0x88
#define MOV_RM16_R16    0x89
#define MOV_RM32_R32    0x89

// RM
#define MOV_R8_RM8      0x8A
#define MOV_R16_RM16    0x8B
#define MOV_R32_RM32    0x8B

// OI 
#define MOV_R8_IMM8     0xB0
#define MOV_R16_IMM16   0xB8
#define MOV_R32_IMM32   0xB8

// MI 

#endif // ASSEMBLER_MOVE_H
