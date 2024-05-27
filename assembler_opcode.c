/** x86 assembler
 * https://www.x.com/7etsuo
 */

#include <stdbool.h>
#include <stdint.h>

#include "./include/assembler_opcode.h"
#include "./include/assembler_mov.h"

void
init_opdata(struct OpData* op)
{
    op->rm.mod_code = op->rm.register_code = op->rm.rm_code = op->rm.rm_word_size = -1;
    op->sib.base_reg_code = op->sib.index_reg_code = op->sib.ss_code = -1;

    for (size_t i = 0; i < ARR_LIT_SZ; i++) {
        op->flags_array_literal[i] = false;
        op->values_array_literal[i] = 0;
    }
}


void
get_instruction_operation_code(struct OpData* operands, uint32_t op_code)
{
    uint32_t aux;
    uint8_t byte;
    int i;

    // mod rm
    if (operands->flags_array_literal[ARR_LIT_MOD]) {
        operands->values_array_literal[ARR_LIT_MOD] = (char)MODR_M(operands->rm.mod_code,
                                                                   operands->rm.rm_code, operands->rm.register_code);
    }

    // SIB  
    if (operands->flags_array_literal[ARR_LIT_SIB]) {
        operands->values_array_literal[ARR_LIT_SIB] = (char)SIB(operands->sib.ss_code,
                                                                operands->sib.index_reg_code, operands->sib.base_reg_code);
    }

    if (op_code > 0xff && op_code <= 0xffff)
        op_code = (op_code >> 8) | (0xff00) & (op_code << 8);

    // opcode
    for (int i = 0; op_code > 0 && i < 4; i++) {
        operands->flags_array_literal[ARR_LIT_OPC_BYTE_THREE - i] = true;
        operands->values_array_literal[ARR_LIT_OPC_BYTE_THREE - i] = (u_int8_t)((op_code) & 0xff);
        op_code >>= 8;
    }
}


long
get_mov_opcode(const struct OpData* operands)
{
    long op_code = -1;

    switch (operands->rm.rm_word_size) {
        case 0: // register to register
            op_code = MOV_RM32_R32;
            break;
        case 8:
            if (operands->rm.rm_code == -1)
                op_code = MOV_R8_IMM8;
            else if (operands->rm_operand == 0 && operands->r_operand == 1)
                op_code = MOV_RM8_R8;
            else if (operands->rm_operand == 1 && operands->r_operand == 0)
                op_code = MOV_R8_RM8;
            else // Todo: for moffset etc.   
                op_code = 0;
            break;
        case 16: // same as case 32 values however, behavior may differe on different 
            if (operands->rm.rm_code == -1)
                op_code = MOV_R16_IMM16;
            else  if (operands->rm_operand == 0 && operands->r_operand == 1)
                op_code = MOV_RM16_R16;
            else if (operands->rm_operand == 1 && operands->r_operand == 0)
                op_code = MOV_R16_RM16;
            else // Todo: for moffset etc.   
                op_code = 0;
            break;
        case 32:
            if (operands->rm.rm_code == -1)
                op_code = MOV_R32_IMM32;
            else  if (operands->rm_operand == 0 && operands->r_operand == 1)
                op_code = MOV_RM32_R32;
            else if (operands->rm_operand == 1 && operands->r_operand == 0)
                op_code = MOV_R32_RM32;
            else // Todo: for moffset etc.
                op_code = 0;
            break;
        default:
            op_code = 0;
            break;
    }
    return op_code;
}
