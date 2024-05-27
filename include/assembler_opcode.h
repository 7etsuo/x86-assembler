/** x86 assembler
 * https://www.x.com/7etsuo
 */

#ifndef ASSEMBLER_OPCODE_H
#define ASSEMBLER_OPCODE_H

#include <stdio.h>                  // FILE*
#include <stdbool.h>
#include <stdlib.h>                 // int 8 
#include "assembler_modrm_sib.h"    // RM, SIB structs

// Array Literal Offsets
#define ARR_LIT_SZ               17

#define ARR_LIT_PRE_GROUP_FOUR   16
#define ARR_LIT_PRE_GROUP_THREE  15
#define ARR_LIT_PRE_GROUP_TWO    14
#define ARR_LIT_PRE_GROUP_ONE    13 // REX.W preceeds the opcode always ***

#define ARR_LIT_OPC_BYTE_THREE   12
#define ARR_LIT_OPC_BYTE_TWO     11
#define ARR_LIT_OPC_BYTE_ONE     10

#define ARR_LIT_MOD              9
#define ARR_LIT_SIB              8

#define ARR_LIT_DISP_BYTE_FOUR   7
#define ARR_LIT_DISP_BYTE_THREE  6
#define ARR_LIT_DISP_BYTE_TWO    5
#define ARR_LIT_DISP_BYTE_ONE    4

#define ARR_LIT_IMM_BYTE_FOUR    3
#define ARR_LIT_IMM_BYTE_THREE   2
#define ARR_LIT_IMM_BYTE_TWO     1
#define ARR_LIT_IMM_BYTE_ONE     0

struct OpData {
    struct RM rm;
    struct SIB sib;
    int rm_operand, r_operand;      // operand1 = 0, operand2 = 1

    u_int8_t values_array_literal[ARR_LIT_SZ];
    bool flags_array_literal[ARR_LIT_SZ];
};

void
get_instruction_operation_code(struct OpData* operands, uint32_t op_code);
long
get_mov_opcode(const struct  OpData* operands);
void
init_opdata(struct OpData* op);

#endif // ASSEMBLER_OPCODE_H
