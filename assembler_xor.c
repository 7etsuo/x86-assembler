/** x86 assembler
 * https://www.x.com/7etsuo
 */

#include "./include/assembler_xor.h"
#include "./include/assembler_sizes.h"
#include "./include/assembler_opcode.h"
#include "./include/assembler_registers.h"
#include "./include/assembler_modrm_sib.h"

#include <stdint.h>
#include <stdlib.h>

int16_t
get_xor_immediate_value_and_opcode(struct OpData* operands, const char* arg1, const char* arg2)
{
    int16_t opcode;
    int32_t imm_register_size, register_size, arg_type;

    if (get_register_code(arg1) == 0) {
        imm_register_size = get_register_size(arg1);
        get_immediate_values(operands, arg2, imm_register_size);
        switch (imm_register_size) {
            case SZ_DISP8:
                opcode = XOR_I_AL_IMM;
                break;
            case SZ_DISP16:
                opcode = XOR_I_AX_IMM;
                break;
            case SZ_DISP32:
                opcode = XOR_I_EAX_IMM;
                break;
            default:
                printf("Immediate value size error\n");
                exit(EXIT_FAILURE);
                break;
        }
        return opcode;
    }

    if (get_argument_type(arg1) != GET_ARG_MEM_FLAG) {
        printf("xor %s %s : dst error\n", arg1, arg2);
        exit(EXIT_FAILURE);
    }

    imm_register_size = get_immediate_size(arg2);
    register_size = get_register_size(arg1);
    get_immediate_values(operands, arg2, imm_register_size);

    if (imm_register_size == SZ_DISP8 && register_size == SZ_DISP8)
        opcode = XOR_MI_RM8_IMM8;
    else if (imm_register_size ^ register_size == 0)
        opcode = XOR_MI_RM32_IMM32; // same as XOR_MI_RM16_IMM16
    else if ((register_size == SZ_DISP32 || register_size == SZ_DISP16) && imm_register_size == SZ_DISP8)
        opcode = XOR_MI_RM32_IMM8;  // same as XOR_MI_RM16_IMM8
    else {
        printf("Immediate value size error\n");
        exit(EXIT_FAILURE);
    }

    return opcode;
}
