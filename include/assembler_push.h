/** x86 assembler
 * https://www.x.com/7etsuo
 */

#ifndef ASSEMBLER_PUSH_H
#define ASSEMBLER_PUSH_H

/*  Opcode* | Instruction | op/en | 64-Bit  | Compat  | Description
  * =================================================================
  *   FF /6  | PUSH r/m16  |   M   | Valid   | Valid  | Push  r/m16.
  *   FF /6  | PUSH r/m32  |   M   | N.E.    | Valid  | Push  r/m32.
  *   FF /6  | PUSH r/m64  |   M   | Valid   | N.E.   | Push  r/m64.
  * -----------------------------------------------------------------
  *   50+rw  | PUSH r16    |   O   | Valid   | Valid  | Push  r16.
  *   50+rd  | PUSH r32    |   O   | N.E.    | Valid  | Push  r32.
  *   50+rd  | PUSH r64    |   O   | Valid   | N.E.   | Push  r64.
  * -----------------------------------------------------------------
  *   6A ib  | PUSH imm8   |   I   | Valid   | Valid  | Push  imm8.
  *   68 iw  | PUSH imm16  |   I   | Valid   | Valid  | Push  imm16.
  *   68 id  | PUSH imm32  |   I   | Valid   | Valid  | Push  imm32.
  * -----------------------------------------------------------------
  *   0E     | PUSH CS     |   ZO  | Invalid | Valid  | Push  CS.
  *   16     | PUSH SS     |   ZO  | Invalid | Valid  | Push  SS.
  *   1E     | PUSH DS     |   ZO  | Invalid | Valid  | Push  DS.
  *   06     | PUSH ES     |   ZO  | Invalid | Valid  | Push  ES.
  *   0F A0  | PUSH FS     |   ZO  | Valid   | Valid  | Push  FS.
  *   0F A8  | PUSH GS     |   ZO  | Valid   | Valid  | Push  GS.
  * =================================================================
  *
  * @ /digit — 
  * A digit between 0 and 7 indicates that the ModR/M byte of the instruction uses only the r/m (register or memory) operand.
  *
  * @ [+rb, +rw, +rd, +ro] — 
  * Indicated the lower 3 bits of the opcode byte is used to encode the register operand
  * without a modR/M byte. The instruction lists the corresponding hexadecimal value of the opcode byte with low
  * 3 bits as 000b. In non-64-bit mode, a register code, from 0 through 7, is added to the hexadecimal value of the
  * opcode byte. In 64-bit mode, indicates the four bit field of REX.b and opcode[2:0] field encodes the register
  * operand of the instruction. “+ro” is applicable only in 64-bit mode. See Table 3-1 for the codes.
  *
  * @ [ib, iw, id, io] — A 1-byte (ib), 2-byte (iw), 4-byte (id) or 8-byte (io) immediate operand to the instruction that
  * follows the opcode, ModR/M bytes or scale-indexing bytes. The opcode determines if the operand is a signed
  * value. All words, doublewords and quadwords are given with the low-order byte first.
  *
  * @ [+rb, +rw, +rd, +ro] — Indicated the lower 3 bits of the opcode byte is used to encode the register operand
  * without a modR/M byte. The instruction lists the corresponding hexadecimal value of the opcode byte with low
  * 3 bits as 000b. In non-64-bit 0E
  *
  * Intel® 64 and IA-32 Architectures - Volume 2 (2A, 2B, 2C & 2D): Instruction Set Reference, A-Z
  * Reference: https://www.intel.com/content/dam/www/public/us/en/documents/manuals
  *
  */

#define PUSH_MOD_VAL    0x06             // for modRM register value

// memory
#define PUSH_M_RM       0xff
#define PUSH_M_RM16     0xff
#define PUSH_M_RM32     0xff
#define PUSH_M_RM64     0xff

// register
#define PUSH_O_REG(reg) (0x50+reg)
#define PUSH_O_R16(reg) (0x50+reg)
#define PUSH_O_R32(reg) (0x50+reg)
#define PUSH_O_R64(reg) (0x50+reg)

// immediate
#define PUSH_I_IMM8     0x6A
#define PUSH_I_IMM16    0x68
#define PUSH_I_IMM32    0x68    

// base 
#define PUSH_ZO_CS      0x0E
#define PUSH_ZO_SS      0x16   
#define PUSH_ZO_DS      0x1E   
#define PUSH_ZO_ES      0x06   
#define PUSH_ZO_FS      0x0FA0
#define PUSH_ZO_GS      0x0FA8

#endif // ASSEMBLER_PUSH_H
