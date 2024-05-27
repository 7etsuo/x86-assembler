#include <stdio.h>
#include <stddef.h> 
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "mov.h"

#define MAX_READ_LINE 256
#define MAX_OPERANDS    3

#define SCHAR_MIN   -128
#define SCHAR_MAX    127
#define SSHORT_MIN   -32768
#define SSHORT_MAX   32767

#define DISP8           8
#define DISP16          16
#define DISP32          32
#define SIB_FOLLOWS     4

#define REGISTER_MAX    7      // change when x64 is implemented 

const char* reg_8[] = { "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh", NULL };
const char* reg_16[] = { "ax", "cx", "dx", "bx", "sp", "bp", "si", "di", NULL };
const char* reg_32[] = { "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", NULL };

enum REG_32 { EAX, ECX, EDX, EBX, ESP, EBP, ESI, EDI };
enum REG_16 { AX, CX, DX, BX, SP, BP, SI, DI };
enum REG_8 { AL, CL, DL, BL, AH, CH, DH, BH };
enum MOD { MOD_00, MOD_01, MOD_10, MOD_11 };

enum REGISTER_TYPE { _REG_TYPE_R_M8, _REG_TYPE_R8, _REG_TYPE_R_R16, _REG_TYPE_R16, _REG_TYPE_R_M32, _REG_TYPE_R32 };

#define MODR_M(mod, rm, r) ((mod<<6) \
+ (rm) \
+ (r << 3))

#define SIB(ss, index, base) ((ss << 6) \
+ (index << 3) \
+ (base)) 

unsigned char test_mod_00[8][8] =
{
    0x00, 0x08, 0x10, 0x18, 0x20, 0x28, 0x30, 0x38,
    0x01, 0x09, 0x11, 0x19, 0x21, 0x29, 0x31, 0x39,
    0x02, 0x0A, 0x12, 0x1A, 0x22, 0x2A, 0x32, 0x3A,
    0x03, 0x0B, 0x13, 0x1B, 0x23, 0x2B, 0x33, 0x3B,
    0x04, 0x0C, 0x14, 0x1C, 0x24, 0x2C, 0x34, 0x3C,
    0x05, 0x0D, 0x15, 0x1D, 0x25, 0x2D, 0x35, 0x3D,
    0x06, 0x0E, 0x16, 0x1E, 0x26, 0x2E, 0x36, 0x3E,
    0x07, 0x0F, 0x17, 0x1F, 0x27, 0x2F, 0x37, 0x3F
};

unsigned char test_mod_01[8][8] =
{
    0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70, 0x78,
    0x41, 0x49, 0x51, 0x59, 0x61, 0x69, 0x71, 0x79,
    0x42, 0x4A, 0x52, 0x5A, 0x62, 0x6A, 0x72, 0x7A,
    0x43, 0x4B, 0x53, 0x5B, 0x63, 0x6B, 0x73, 0x7B,
    0x44, 0x4C, 0x54, 0x5C, 0x64, 0x6C, 0x74, 0x7C,
    0x45, 0x4D, 0x55, 0x5D, 0x65, 0x6D, 0x75, 0x7D,
    0x46, 0x4E, 0x56, 0x5E, 0x66, 0x6E, 0x76, 0x7E,
    0x47, 0x4F, 0x57, 0x5F, 0x67, 0x6F, 0x77, 0x7F
};

unsigned char test_mod_10[8][8] =
{
    0x80, 0x88, 0x90, 0x98, 0xA0, 0xA8, 0xB0, 0xB8,
    0x81, 0x89, 0x91, 0x99, 0xA1, 0xA9, 0xB1, 0xB9,
    0x82, 0x8A, 0x92, 0x9A, 0xA2, 0xAA, 0xB2, 0xBA,
    0x83, 0x8B, 0x93, 0x9B, 0xA3, 0xAB, 0xB3, 0xBB,
    0x84, 0x8C, 0x94, 0x9C, 0xA4, 0xAC, 0xB4, 0xBC,
    0x85, 0x8D, 0x95, 0x9D, 0xA5, 0xAD, 0xB5, 0xBD,
    0x86, 0x8E, 0x96, 0x9E, 0xA6, 0xAE, 0xB6, 0xBE,
    0x87, 0x8F, 0x97, 0x9F, 0xA7, 0xAF, 0xB7, 0xBF
};

unsigned char test_mod_11[8][8] =
{
    0xC0, 0xC8, 0xD0, 0xD8, 0xE0, 0xE8, 0xF0, 0xF8,
    0xC1, 0xC9, 0xD1, 0xD9, 0xE1, 0xE9, 0xF1, 0xF9,
    0xC2, 0xCA, 0xD2, 0xDA, 0xE2, 0xEA, 0xF2, 0xFA,
    0xC3, 0xCB, 0xD3, 0xDB, 0xE3, 0xEB, 0xF3, 0xFB,
    0xC4, 0xCC, 0xD4, 0xDC, 0xE4, 0xEC, 0xF4, 0xFC,
    0xC5, 0xCD, 0xD5, 0xDD, 0xE5, 0xED, 0xF5, 0xFD,
    0xC6, 0xCE, 0xD6, 0xDE, 0xE6, 0xEE, 0xF6, 0xFE,
    0xC7, 0xCF, 0xD7, 0xDF, 0xE7, 0xEF, 0xF7, 0xFF
};

struct RM {
    bool rm_flag;
    int rm_word_size;
    long rm_register_code;
    long register_code;
    int mod_code;
};

struct SIB {
    bool sib_flag;
    long index_reg_code;
    long base_reg_code;
    int ss_code;
};

struct OpData {
    struct RM rm;
    struct SIB sib;
    long displacement;
    int rm_operand, r_operand; // operand1 = 1, operand2 = 2, operand3 = 3. 
};

void init_opdata(struct OpData* op);
void get_mod_sib_imm_data(struct OpData* op, const char* arg1, const char* arg2);
int get_rm_word_size(const char* rm);
int get_scale_size(const char* rm);
long get_displacement_value(const char* rm);
int get_displacement_code(long displacement);
char get_register_code(const char* s);
long get_index_reg_code(const char* rm);
long get_base_reg_code(const char* rm);
int get_mod_code(struct RM rm, long displacement);
int __l0g_2_(long v);

size_t digits_to_hex_string(long digits, char* hex_string, size_t n);
long get_mov_opcode(struct OpData operands);
long get_instruction_operation_code(struct OpData operands, long op_code);

int main(int argc, char* argv[])
{
    const char* instruction = "MOV  EAX, DWORD PTR [EDX + EBX * 4 + 8]";
    const char* mnemonic = "mov";
    const char* arg1 = "ecx";

    const char* arg2 = "dwordptr[edx+ebx*4+8]";
    const char* rm_test_one = "dwordptr[ebx]";
    const char* rm_test_three = "dwordptr[ebx+8]";
    const char* rm_test_two = "dwordptr[ebx*4+8]";

    char hex_string[100]; // big enough. 
    size_t sz_hex_string;

    long mov_op_code, instruction_operation;

    struct OpData operands;

    init_opdata(&operands);
    get_mod_sib_imm_data(&operands, arg1, arg2);

    mov_op_code = get_mov_opcode(operands);
    instruction_operation = get_instruction_operation_code(operands, mov_op_code);
    sz_hex_string = digits_to_hex_string(instruction_operation, hex_string, 100);

    puts(hex_string);

    return 0;
}

long get_mov_opcode(struct OpData operands)
{
    long op_code = -1;

    switch (operands.rm.rm_word_size) {
        this is a test
        case 8:
            if (operands.rm_operand == 1 && operands.r_operand == 2) {
                op_code = MOV_RM8_R8;
            }
            else if (operands.rm_operand == 2 && operands.r_operand == 1) {
                op_code = MOV_R8_RM8;
            }
            else // Todo: for immediate etc.   
            {
                op_code = 0;
            }
            break;
        case 16: // same as case 32 values however, behavior may differe on different 
            if (operands.rm_operand == 1 && operands.r_operand == 2) {
                op_code = MOV_RM16_R16;
            }
            else if (operands.rm_operand == 2 && operands.r_operand == 1) {
                op_code = MOV_R16_RM16;
            }
            else // Todo: for immediate etc.   
            {
                op_code = 0;
            }
            break;
        case 32:
            if (operands.rm_operand == 1 && operands.r_operand == 2) {
                op_code = MOV_RM32_R32;
            }
            else if (operands.rm_operand == 2 && operands.r_operand == 1) {
                op_code = MOV_R32_RM32;
            }
            else // Todo: for immediate etc.   
            {
                op_code = 0;
            }
            break;
        default:
            op_code = 0;
            break;
    }

    return op_code;
}

long get_instruction_operation_code(struct OpData operands, long op_code)
{
    long mod_code, sib_code, instruction_operation;

    mod_code = MODR_M(operands.rm.mod_code, operands.rm.rm_register_code, operands.rm.register_code);
    if (operands.sib.sib_flag) {
        sib_code = SIB(operands.sib.ss_code, operands.sib.index_reg_code, operands.sib.base_reg_code);
    }

    instruction_operation = op_code;
    if (operands.sib.sib_flag) {
        if (operands.displacement != 0) {
            instruction_operation <<= 24;
            instruction_operation |= mod_code << 16;
            instruction_operation |= sib_code << 8;
            instruction_operation |= operands.displacement;
        }
        else {
            instruction_operation <<= 16;
            instruction_operation |= mod_code << 8;
            instruction_operation |= sib_code;
        }
    }
    else {
        instruction_operation <<= 8;
        instruction_operation |= mod_code;
    }

    return instruction_operation;
}

size_t digits_to_hex_string(long digits, char* hex_string, size_t n)
{
    size_t i, j, k;
    long q, r;

    i = 0;
    if (digits == 0) {
        hex_string[i++] = 0x30;
    }
    // (x = q*16 + r) recursive
    for (; digits != 0; i++) {
        q = digits >> 4;
        digits -= (q << 4);

        switch (digits) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                hex_string[i] = (digits + 0x30);
                break;
            case 10:
                hex_string[i] = 'a';
                break;
            case 11:
                hex_string[i] = 'b';
                break;
            case 12:
                hex_string[i] = 'c';
                break;
            case 13:
                hex_string[i] = 'd';
                break;
            case 14:
                hex_string[i] = 'e';
                break;
            case 15:
                hex_string[i] = 'f';
                break;
            default: // wut ?
                break;
        }
        digits = q;
    }
    hex_string[i] = '\0';

    j = 0;
    k = (i - 1);
    for (;j < k; j++, k--) {
        hex_string[j] ^= hex_string[k];
        hex_string[k] ^= hex_string[j];
        hex_string[j] ^= hex_string[k];
    }
    return i;
}

void init_opdata(struct OpData* op)
{
    op->displacement = 0;
    op->rm.mod_code = op->rm.register_code = op->rm.rm_register_code = op->rm.rm_word_size = -1;
    op->sib.base_reg_code = op->sib.index_reg_code = op->sib.ss_code = -1;
    op->rm.rm_flag = op->sib.sib_flag = false;
}

void get_mod_sib_imm_data(struct OpData* op, const char* arg1, const char* arg2)
{
    const char* rm_arg, * r_arg;
    int scale_size;
    size_t sz_a1, sz_a2;

    op->rm.rm_word_size = get_rm_word_size(arg1);
    if (op->rm.rm_word_size == 0) // need to find RM 
    {
        r_arg = arg1;
        op->r_operand = 1;
        rm_arg = arg2;
        op->rm_operand = 2;
        op->rm.rm_word_size = get_rm_word_size(rm_arg);
    }
    else // rm is either arg2 or they are both registers. 
    {
        r_arg = arg2;
        op->r_operand = 2;
        rm_arg = arg1;
        op->rm_operand = 1;
        op->rm.rm_word_size = get_rm_word_size(rm_arg);
    }

    op->rm.rm_flag = (op->rm.rm_word_size != 0); // if false then both are registers.
    op->displacement = get_displacement_value(rm_arg);
    op->rm.mod_code = get_mod_code(op->rm, op->displacement);

    if (!op->rm.rm_flag) // Todo: deal with immediate 
    {
        op->rm_operand = 2;
        op->r_operand = 1;

        op->rm.register_code = get_register_code(r_arg);
        op->rm.rm_register_code = get_register_code(rm_arg);
    }
    else {
        op->rm.register_code = get_register_code(r_arg);

        op->sib.index_reg_code = get_index_reg_code(rm_arg);
        op->sib.sib_flag = (op->sib.index_reg_code != 0);
        op->rm.rm_register_code = op->sib.sib_flag ? SIB_FOLLOWS : get_base_reg_code(rm_arg);

        if (op->sib.sib_flag) {
            scale_size = get_scale_size(rm_arg);
            op->sib.ss_code = __l0g_2_(scale_size + 1);
            op->sib.base_reg_code = get_base_reg_code(rm_arg);
        }
    }
}

int get_mod_code(struct RM rm, long displacement)
{
    int mod_code = -1;

    if (rm.rm_word_size == 0) {
        mod_code = MOD_11;
    }
    else if (displacement == 0) {
        mod_code = MOD_00;

    }
    else if (displacement <= SCHAR_MAX && displacement >= SCHAR_MIN) {
        mod_code = MOD_01;
    }
    else {
        mod_code = MOD_10;
    }

    return mod_code;
}

long get_index_reg_code(const char* rm)
{
    char base[4];
    int code = 0;
    size_t i = 0;
    while (rm[i++] != '[') {
        if (rm[i] == '\0') {
            return 0;
        }
    }

    for (i; rm[i] != '\0'; i++) {
        if (isalpha(rm[i]) && rm[i + 1] != '\0' && rm[i + 2] != '\0') {
            if (rm[i] == 'x') // is an offset
            {
                return get_displacement_value(&rm[i]);
            }

            if (!isalpha(rm[i + 2]) && rm[i + 2] != '*') // is base
            {
                i += 2;
                continue;
            }

            if (rm[i + 3] != '*') {
                i += 3;
                continue;
            }

            base[0] = rm[i];
            base[1] = rm[i + 1];
            if (isalpha(rm[i + 2])) {
                base[2] = rm[i + 2];
                base[3] = '\0';
            }
            else {
                base[2] = '\0';
            }

            return get_register_code(base);
        }
    }
    return -1;
}

long get_base_reg_code(const char* rm)
{
    char base[4];
    int code = 0;
    size_t i = 0;
    while (rm[i++] != '[') {
        if (rm[i] == '\0') {
            return 0;
        }
    }

    for (i; rm[i] != '\0'; i++) {
        if (isalpha(rm[i]) && rm[i + 1] != '\0' && rm[i + 2] != '\0') {
            if (rm[i] == 'x') // is an offset
            {
                return get_displacement_value(&rm[i]);
            }

            if (rm[i + 2] == '*') // is index
            {
                i += 2;
                continue;
            }

            if (rm[i + 3] == '*') {
                i += 3;
                continue;
            }

            base[0] = rm[i];
            base[1] = rm[i + 1];
            if (isalpha(rm[i + 2])) {
                base[2] = rm[i + 2];
                base[3] = '\0';
            }
            else {
                base[2] = '\0';
            }

            return get_register_code(base);
        }
    }
    return -1;
}

long get_displacement_value(const char* rm)
{
    long ret_val = 0;
    char* endptr;
    for (size_t i = 0; rm[i]; i++) {
        if (rm[i] == '+' || rm[i] == '-') // 
        {
            ret_val = strtol(&rm[i], &endptr, 0);
            if (endptr == (&rm[i])) {
                continue;
            }
            return ret_val;
        }
    }
    return ret_val;
}

int get_scale_size(const char* rm)
{
    int scale_size;
    for (size_t i = 0; rm[i]; i++) {
        if (rm[i] == '*' && isdigit(rm[i + 1])) {
            scale_size = rm[i + 1] - 0x30;
            return (scale_size < 0 || scale_size > 8) ? -1 : scale_size;
        }

        if (rm[i] == '*') // *?
        {
            return -1;
        }
    }

    return 0; // no scale
}

int get_rm_word_size(const char* rm)
{
    char c = rm[0];
    int ret_val = 0; // is a register. 
    switch (c) {
        case 'b':
            ret_val = 8;
            break;
        case 'w':
            ret_val = 16;
            break;
        case 'd':
            ret_val = 32;
            break;
        case '[': // no size specified
            ret_val = -1;
            break;
        default:
            break;
    }
    return ret_val;
}


char get_register_code(const char* s) // 3 bit register code
{
    char c1, c2, reg_code = -1;

    if (s[0] != 'e' && s[2] != '\0') // TODO: add 64 bit support
    {
        return reg_code;
    }

    if (s[2] == '\0') {
        c1 = s[0];
        c2 = s[1];
    }
    else {
        c1 = s[1];
        c2 = s[2];
    }

    switch (c1) {
        case 'a':
            if (c2 == 'h') {
                reg_code = 4;
            }
            else if (c2 == 'x' || c2 == 'l') {
                reg_code = 0;
            }
            break;
        case 'c':
            if (c2 == 'h') {
                reg_code = 5;
            }
            else if (c2 == 'x' || c2 == 'l') {
                reg_code = 1;
            }
            break;
        case 'd':
            if (c2 == 'h') {
                reg_code = 6;
            }
            else if (c2 == 'x' || c2 == 'l') {
                reg_code = 2;
            }
            else if (c2 == 'i') {
                reg_code = 7;
            }
            break;
        case 'b':
            if (c2 == 'h') {
                reg_code = 7;
            }
            else if (c2 == 'x' || c2 == 'l') {
                reg_code = 3;
            }
            else if (c2 == 'p') {
                reg_code = 5;
            }
            break;
        case 's':
            if (c2 == 'i') {
                reg_code = 6;
            }
            else if (c2 == 'p') {
                reg_code = 4;
            }
            break;
        default:    // ...
            break;
    }
    return reg_code;
}

int __l0g_2_(long v)
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
