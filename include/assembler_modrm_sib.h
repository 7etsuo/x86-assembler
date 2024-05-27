/** x86 assembler
 * https://www.x.com/7etsuo
 */

#ifndef MODRM_SIB_H
#define MODRM_SIB_H

#include "assembler_sizes.h"
#include <stdint.h>
#include <stdbool.h>

// for get_register_flag()
#define GET_ARG_REG_FLAG  0
#define GET_ARG_IMM_FLAG  1
#define GET_ARG_MEM_FLAG  2

#define SIB_FOLLOWS       4


enum MOD { MOD_00, MOD_01, MOD_10, MOD_11 };

#define MODR_M(mod, rm, r) ((mod<<6) \
    + (rm) \
    + (r << 3))
#define SIB(ss, index, base) ((ss << 6) \
    + (index << 3) \
    + (base)) 

struct RM {
    int rm_word_size;
    long rm_code;
    long register_code;
    int mod_code;
};

struct SIB {
    long index_reg_code;
    long base_reg_code;
    int ss_code;
};

struct OpData; // forward declaration

bool
get_mod_data_set_sib_flag(struct OpData* op, const char* arg1, const char* arg2);
void
get_mod_sib_imm_data(struct OpData* op, const char* arg1, const char* arg2);
void
get_immediate_values(struct OpData* op, const char* rm_arg, int imm_sz);
int32_t
get_displacement_value_ret_nbits(struct OpData* op, const char* rm);
void
get_mod_code_and_displace_sz(struct OpData* op, const char* arg);
void
get_rm_code_set_sib_flag(struct OpData* op, const char* arg);
void
get_sib_data(struct OpData* op, const char* argument);
int32_t
get_argument_type(const char* argument);
int32_t
get_immediate_size(const char* arg);
int32_t
get_index_reg_code(const char* rm);
int32_t
get_base_reg_code(const char* rm);
int32_t
get_rm_word_size(const char* rm);
int32_t
get_scale_size(const char* rm);
int32_t
__l0g_2_(long v);

#endif // MODRM_SIB_H
