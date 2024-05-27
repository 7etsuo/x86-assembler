/** x86 assembler
 * https://www.x.com/7etsuo
 */

#ifndef ASSEMBLER_JMP_H
#define ASSEMBLER_JMP_H

/* ****** JMP—Jump
  *
  * ===========================================================================================================================================================
  *          | Opcode|                   |en| 64-Bit  |  Leg |                                    | Mode Compat/ Description
  * ===========================================================================================================================================================
  *          | EB cb | JMP | rel8        | D | Valid | Valid  | Jump |short,|                    | RIP = RIP + 8-bit displacement sign extended to 64-bits
  *          | E9 cw | JMP | rel16       | D | N.S.  | Valid  | Jump |near, |                    | relative, displacement relative to next instruction. Not in 64
  *          | E9 cd | JMP | rel32       | D | Valid | Valid  | Jump |near, | relative,          | RIP = RIP + 32-bit displacement sign extended to 64-bits
  *          | FF /4 | JMP | r/m16       | M | N.S.  | Valid  | Jump |near, | absolute indirect, | address = zero- extended r/m16. Not supported in 64-bit mode
  *          | FF /4 | JMP | r/m32       | M | N.S.  | Valid  | Jump |near, | absolute indirect, | address given in r/m32. Not supported in 64-bit mode.
  *          | FF /4 | JMP | r/m64       | M | Valid | N.E.   | Jump |near, | absolute indirect, | RIP = 64-Bit offset from register or memory
  *          | EA cd | JMP | ptr16:16    | S | Inv.  | Valid  | Jump |far,  | absolute,          | address given in operand
  *          | EA cp | JMP | ptr16:32    | S | Inv.  | Valid  | Jump |far,  | absolute,          | address given in operand
  *          | FF /5 | JMP | m16:16      | M | Valid | Valid  | Jump |far,  | absolute indirect, | address given in m16:16
  *          | FF /5 | JMP | m16:32      | M | Valid | Valid  | Jump |far,  | absolute indirect, | address given in m16:32.
  *  REX.W   | FF /5 | JMP | m16:64      | M | Valid | N.E.   | Jump |far,  | absolute indirect, | address given in m16:64
  *  ===========================================================================================================================================================
  *
  * - Near jump—A jump to an instruction within the current code segment (the segment currently pointed to by the CS register), sometimes referred to as an intrasegment jump.
  * - Short jump—A near jump where the jump range is limited to –128 to +127 from the current EIP value.
  * - Far jump—A jump to an instruction located in a different segment than the current code segment but at the same privilege level, sometimes referred to as an intersegment jump.
  * - Task switch—A jump to an instruction located in a different task.
  *
  * cb, cw, cd, cp, co, ct — 
  * 1-byte  (cb),
  * 2-byte  (cw),
  * 4-byte  (cd),
  * 6-byte  (cp),
  * 8-byte  (co) or
  * 10-byte (ct) value following the opcode.
  * This value is used to specify a code offset and possibly a new value for the code segment register.
  *
  *  INSTRUCTION SET REFERENCE, A-L
  *  Intel® 64 and IA-32 Architectures - Volume 2 (2A, 2B, 2C & 2D):
  *  Reference: https://www.intel.com/content/dam/www/public/us/en/documents/manuals/64-ia-32-architectures-software-developer-instruction-set-reference-manual-325383.pdf
  *
  */

#define JMP_D_SHORT(x)          (0xEB << 8 ) | (0xff & (x)) // cb
#define JMP_D_NEAR              0xE9                        // cw
#define JMP_D_NEAR_REL          0xE9                        // cd

#define JMP_M8_NEAR_ABS_IND     0xFF    // /4
#define JMP_M16_NEAR_ABS_IND    0xFF    // /4
#define JMP_M32_NEAR_ABS_IND    0xFF    // /4

#define JMP_S_PTR_16_16_ABS     0xEA    // cd
#define JMP_S_PTR_16_32_ABS     0xEA    // cp

#define JMP_M_16_16             0xFF    // /5
#define JMP_M_16_32             0xFF    // /5
#define JMP_M_16_64             0xFF    // /5

#endif // ASSEMBLER_JMP_H
