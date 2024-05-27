/** x86 assembler
 * https://www.x.com/7etsuo
 */

#include <stdlib.h>
#include <stdio.h> 
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

#include "./include/assembler_modrm_sib.h"
#include "./include/assembler_registers.h"
#include "./include/assembler_opcode.h"
#include "./include/assembler_sizes.h"

int32_t
get_argument_type(const char* argument)
{
    if (argument == NULL)
        return -1;

    if (get_rm_word_size(argument) > 0)
        return GET_ARG_MEM_FLAG;    // GET_ARG_MEM_FLAG = 2

    // GET_ARG_IMM_FLAG = 1
    return (get_register_code(argument) < 0);   // GET_ARG_REG_FLAG = 0 
}

/*
 * case1:  r, imm
 * case2: rm, imm
 * case3:  r, r
 * case4: rm, r
 * case5:  r, rm
 */

void
get_mod_sib_imm_data(struct OpData* op, const char* arg1, const char* arg2)
{
    if (!arg1 || !arg2)
        return;

    if (get_mod_data_set_sib_flag(op, arg1, arg2) == false) { // immediate value
        op->rm.rm_word_size = get_register_size(arg1);
        get_immediate_values(op, arg2, op->rm.rm_word_size);
    }

    if (op->flags_array_literal[ARR_LIT_SIB]) {
        if (op->rm_operand) // if 1 rm is arg2. 
            get_sib_data(op, arg2);
        else // rm is arg1
            get_sib_data(op, arg1);
    }
}


bool
get_mod_data_set_sib_flag(struct OpData* op, const char* arg1, const char* arg2)
{
    int arg1_type, arg2_type;
    const char* rm_val, * r_val;

    arg1_type = get_argument_type(arg1);
    if (arg1_type == -1)
        return false;

    arg2_type = get_argument_type(arg2);
    if (arg2_type == -1)
        return false;

    if (arg1_type == GET_ARG_IMM_FLAG) {
        printf("dst %s can't be an immediate\n", arg1);
        exit(EXIT_FAILURE);
    }

    if (arg2_type == GET_ARG_IMM_FLAG) // no mod
        return false;

    if (arg1_type == GET_ARG_REG_FLAG) {
        op->r_operand = 0;
        op->rm_operand = 1;
        rm_val = arg2;
        r_val = arg1;
    }
    else { // arg1_type == GET_ARG_MEM_FLAG 
        op->r_operand = 1;
        op->rm_operand = 0;
        rm_val = arg1;
        r_val = arg2;
    }

    get_mod_code_and_displace_sz(op, rm_val);
    op->rm.register_code = get_register_code(r_val);
    get_rm_code_set_sib_flag(op, rm_val);

    return true;
}


void
get_sib_data(struct OpData* op, const char* argument)
{
    op->sib.index_reg_code = get_index_reg_code(argument);
    op->sib.ss_code = __l0g_2_(get_scale_size(argument) + 1);
    op->sib.base_reg_code = get_base_reg_code(argument);
}


void
get_mod_code_and_displace_sz(struct OpData* op, const char* arg)
{
    int word_size, displacement;

    op->rm.rm_word_size = get_rm_word_size(arg);
    displacement = get_displacement_value_ret_nbits(op, arg);
    op->rm.mod_code = -1;

    if (op->rm.rm_word_size == 0 && displacement == 0)
        op->rm.mod_code = MOD_11;
    else if (displacement == 0)
        op->rm.mod_code = MOD_00;
    else if (displacement == 8)
        op->rm.mod_code = MOD_01;
    else if (displacement == 16 || displacement == 32)
        op->rm.mod_code = MOD_10;


    op->flags_array_literal[ARR_LIT_MOD] = (op->rm.mod_code != -1);
}


void
get_rm_code_set_sib_flag(struct OpData* op, const char* arg)
{
    if (get_index_reg_code(arg) == -1)
        op->rm.rm_code = get_base_reg_code(arg);
    else {
        op->rm.rm_code = SIB_FOLLOWS;
        op->flags_array_literal[ARR_LIT_SIB] = true;
    }
}


int32_t
get_index_reg_code(const char* rm)
{
    int32_t i, rmcpy_sz, code;
    char* rmcpy;

    i = 0;
    while (rm[i++] != '[') {
        if (rm[i] == '\0')
            return -1;
    }
    rm += i;

    i = 0;
    while (rm[i++] != '+') {
        if (rm[i] == '\0')
            return -1;
    }
    rm += i;

    rmcpy = (char*)malloc(i);
    memcpy(rmcpy, rm, i);
    rmcpy[i - 1] = '\0';

    code = get_register_code(rmcpy);
    free(rmcpy);

    return code;
}


int32_t
get_base_reg_code(const char* rm)
{
    int32_t i, code;
    char* rmcpy, c;

    i = 0;
    while (rm[i++] != '[') {
        if (rm[i] == '\0')
            return 0;
    }
    rm += i;

    i = 0;
    while (isalpha(rm[i++]))
        ;

    rmcpy = (char*)malloc(i);
    memcpy(rmcpy, rm, i);
    rmcpy[i - 1] = '\0';


    code = get_register_code(rmcpy);
    free(rmcpy);


    return code;

}


int32_t
get_scale_size(const char* rm)
{
    char* endptr;
    int32_t scale_value;

    for (size_t i = 0; rm[i]; i++) {
        if (rm[i] == '*' && isdigit(rm[i + 1])) {
            scale_value = strtol(&rm[i + 1], &endptr, 0);
            if (scale_value == 1)
                scale_value = 0;


            if (scale_value > 8 || scale_value % 2 != 0) {
                printf("Invalid scale size %d\n", scale_value);
                exit(EXIT_FAILURE);
            }
            return scale_value;
        }

        if (rm[i] == '*') // *?
            return -1;

    }

    return 0; // no scale
}


int32_t
get_rm_word_size(const char* rm)
{
    char rmcp[MAX_READ_LINE];
    int hash_value, i;
    for (i = 0; rm[i] != '[' && rm[i] != '\0'; i++);
    if (rm[i] != '[') // no size 
        return 0;
    memcpy(rmcp, rm, i);
    rmcp[i] = '\0';

    return get_register_size(rmcp);
}


int32_t
get_immediate_size(const char* arg)
{
    char* endptr;
    int imm_sz;
    int32_t immediate;

    immediate = strtol(arg, &endptr, 0);
    if (endptr == arg) {
        fprintf(stderr, "immediate values error %s\n", arg);
        exit(EXIT_FAILURE);
    }

    if (immediate >= SCHAR_MIN && immediate <= SCHAR_MAX) // 0xff - 0x7f
        imm_sz = SZ_DISP8;
    else if (immediate <= 0x7fff)
        imm_sz = SZ_DISP16;
    else
        imm_sz = SZ_DISP32;

    return imm_sz;
}


void
get_immediate_values(struct OpData* op, const char* rm_arg, int imm_sz)
{
    char* endptr;
    int32_t immediate;

    immediate = strtol(rm_arg, &endptr, 0);
    if (endptr == rm_arg) {
        fprintf(stderr, "immediate values error %s, %d\n", rm_arg, imm_sz);
        exit(EXIT_FAILURE);
    }

    imm_sz = imm_sz > 8 ? imm_sz : imm_sz >> 3;
    for (int i = ARR_LIT_IMM_BYTE_FOUR; i >= 0 && imm_sz > 0; i--, imm_sz--) {
        op->flags_array_literal[i] = true;
        op->values_array_literal[i] = (u_int8_t)(immediate & 0xff);
        immediate >>= 8;
    }
}


int32_t
get_displacement_value_ret_nbits(struct OpData* op, const char* rm)
{
    int32_t val;
    char* endptr;
    int i, n_bytes, wordsize, bit_size_flag, n_bit_displacement;

    val = 0;
    for (i = 0; rm[i]; i++) {
        if (rm[i] == '+' || rm[i] == '-') // 
        {
            val = strtol(&rm[i], &endptr, 0);
            if (endptr == (&rm[i])) // was a register *may have to fix for address's 
                continue;
            break;
        }
    }

    if (val == 0)
        return val;

    n_bytes = 0;
    if (val > 0 && val <= 0x7f) {
        n_bit_displacement = SZ_DISP8;
        n_bytes = 1;
    }
    else if (val > 0x7f) {
        n_bytes = 4;
        n_bit_displacement = SZ_DISP32;
    }

    for (i = ARR_LIT_DISP_BYTE_FOUR; n_bytes != 0; i--, n_bytes--) {
        op->flags_array_literal[i] = true;
        op->values_array_literal[i] = (u_int8_t)(val & 0xff);
        val >>= 8;
    }

    return n_bit_displacement;
}


int
__l0g_2_(long v)
{
    static const int MultiplyDeBruijnBitPosition[32] =
        {
            0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3, 30,
            8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31
        };

    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;

    return MultiplyDeBruijnBitPosition[(long)(v * 0x07C4ACDDU) >> 27];
}
