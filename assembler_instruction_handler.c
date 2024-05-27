/** x86 assembler
 * https://www.x.com/7etsuo
 */

#include <stdlib.h> 
#include <stdio.h> 
#include <stdbool.h> 
#include <string.h> 
#include <ctype.h>

#include "./include/assembler_instruction_handler.h"
#include "./include/assembler_registers.h"
#include "./include/assembler_opcode.h"
#include "./include/assembler_sysmov.h"
#include "./include/assembler_cuckoo.h"
#include "./include/assembler_sizes.h"
#include "./include/assembler_push.h"
#include "./include/assembler_xor.h"
#include "./include/assembler_jmp.h"

struct {
    const char* k;
    void (*v)(const struct Instruction* ins, char* hex_string);
} table[] = {
    { "sysmov"   , &handle_sysmov },
    { "push"     , &handle_push   },
    { "mov"      , &handle_mov    },
    { "nop"      , &handle_nop    },
    { "int"      , &handle_int    },
    { "xor"      , &handle_xor    },
    { "jmp"      , &handle_jmp    },
    { NULL       , NULL           },
};

uint32_t g_current_byte_offset = 0L;
uint32_t g_total_bytes = 0L;

void
(*lookup_handler(const char* mnemonic))(const struct Instruction* ins, char* hex_string)
{
    const char* kptr;
    for (size_t i = 0; (kptr = table[i].k) != NULL; i++) {
        if (!strncmp(kptr, mnemonic, strlen(mnemonic)))
            return table[i].v;
    }

    return NULL;
}

/******** Instruction: jmp
 *  - jmp label
 */

void
handle_jmp(const struct Instruction* ins, char* hex_string)
{
    struct OpData operands;
    HTENTRY* hptr;
    const char* argptr = ins->argument[0];
    int32_t opcode;

    init_opdata(&operands);
    if (ins->first_pass_flag) {
        memcpy(hex_string, "0x0x", strlen("0x0x") + 1); // pad 2 bytes (short jump only)
        return;
    }

    hptr = hash_lookup(argptr);
    if ((hptr) == NULL)
        fprintf(stderr, "jmp %s not currently supported. \n", argptr);


    g_current_byte_offset += 2;
    opcode = JMP_D_SHORT((hptr->byte_num - g_current_byte_offset));
    g_current_byte_offset -= 2;
    get_instruction_operation_code(&operands, (uint16_t)opcode);
    digits_to_hex_string(&operands, hex_string);
}

/******** Instruction: xor
 * - xor AL, Imm
 * - xor AX, Imm
 * - xor EAX,Imm
 * - xor RM, Imm
 * - xor RM, R
 * - xor R,  RM
 */

void
handle_xor(const struct Instruction* ins, char* hex_string)
{
    int16_t arg2_type, arg1_type, opcode, arg2_size, arg1_size, eax_flag;
    struct OpData operands;
    const char* arg1_ptr, * arg2_ptr;

    init_opdata(&operands);

    arg1_ptr = ins->argument[0];
    arg2_ptr = ins->argument[1];

    arg2_type = get_argument_type(arg2_ptr);
    arg1_type = get_argument_type(arg1_ptr);

    arg2_size = get_rm_word_size(arg2_ptr);
    arg1_size = get_rm_word_size(arg1_ptr);

    if (arg2_type == GET_ARG_IMM_FLAG) {
        opcode = get_xor_immediate_value_and_opcode(&operands, arg1_ptr, arg2_ptr);
        eax_flag = 1;

        if (arg1_type == GET_ARG_MEM_FLAG) {
            arg1_ptr = reg_32[ESI];
            arg2_ptr = ins->argument[0];
            eax_flag = 0;
        }

        if (eax_flag == 1)
            goto compute_instruction;

    }
    else if (arg1_type == GET_ARG_REG_FLAG && arg2_type == GET_ARG_MEM_FLAG) {
        switch (arg2_size) {
            case SZ_DISP8:
                opcode = XOR_RM_R8_RM8;
                break;
            case SZ_DISP16:
                opcode = XOR_RM_R16_RM16;
                break;
            case SZ_DISP32:
                opcode = XOR_RM_R32_RM32;
                break;
        }
    }
    else if ((arg1_type == GET_ARG_MEM_FLAG || arg1_type == GET_ARG_REG_FLAG) && arg2_type == GET_ARG_REG_FLAG) {
        if (arg2_type == GET_ARG_REG_FLAG && arg1_type == GET_ARG_REG_FLAG) // register to register
            arg1_size = get_register_size(arg1_ptr);
        switch (arg1_size) {
            case SZ_DISP8:
                opcode = XOR_MR_RM8_R8;
                break;
            case SZ_DISP16:
                opcode = XOR_MR_RM16_R16;
                break;
            case SZ_DISP32:
                opcode = XOR_MR_RM32_R32;
                break;
        }
    }
    else {
        printf("xor %s %s not supported", ins->argument[0], ins->argument[1]);
        exit(EXIT_FAILURE);
    }

    get_mod_sib_imm_data(&operands, arg1_ptr, arg2_ptr);

compute_instruction:
    get_instruction_operation_code(&operands, opcode);
    digits_to_hex_string(&operands, hex_string);
}

/******** Instruction: sysmov
 * - sysmov sys_call
 * equivalent to mov eax, syscall_value
 */

void
handle_sysmov(const struct Instruction* ins, char* hex_string)
{
    int8_t eax_value;
    void (*handle_mnemonic)(const struct Instruction* ins, char* hex_string);
    struct Instruction ins_new;

    eax_value = get_sysmov_eax_value(ins->argument[0]);
    if (eax_value == -1) {
        printf("%s is not a valid sysmov argument", ins->argument[0]);
        exit(EXIT_FAILURE);
    }

    memcpy(ins_new.argument[0], "eax", sizeof("mov"));
    sprintf(ins_new.argument[1], "%d", eax_value);

    handle_mnemonic = lookup_handler("mov");
    handle_mnemonic(&ins_new, hex_string);
}

/******** Instruction: push
 * - PUSH r/m
 * - PUSH imm
 * - PUSH r
 * - PUSH CS, DS, ES, FS or GS
 */

void
handle_push(const struct Instruction* ins, char* hex_string)
{
    struct OpData operands;
    int opcode, imm_sz, reg_code;
    const char* arg;

    init_opdata(&operands);
    arg = ins->argument[0];

    opcode = -1;
    switch (get_argument_type(arg)) {
        case GET_ARG_MEM_FLAG:
            {
                opcode = PUSH_M_RM;
                get_mod_sib_imm_data(&operands, "esi", arg);
                break;
            }
        case GET_ARG_IMM_FLAG:
            {
                imm_sz = get_immediate_size(arg);
                switch (imm_sz) {
                    case SZ_DISP32:
                        opcode = PUSH_I_IMM32;
                        break;
                    case SZ_DISP16:
                        opcode = PUSH_I_IMM16;
                        break;
                    case SZ_DISP8:
                        opcode = PUSH_I_IMM8;
                        break;
                    default:
                        printf("in push %s size not yet supported\n", arg);
                        exit(EXIT_FAILURE);
                        break;
                }
                get_immediate_values(&operands, arg, imm_sz);
                break;
            }
        case GET_ARG_REG_FLAG:
        default:
            {
                reg_code = get_register_code(arg);
                switch (get_register_type(arg)) {
                    case REG_GP:
                        opcode = PUSH_O_REG(reg_code);
                        break;
                    case REG_SEGMENT:
                        switch (reg_code) {
                            case CS:
                                opcode = PUSH_ZO_CS;
                                break;
                            case DS:
                                opcode = PUSH_ZO_DS;
                                break;
                            case SS:
                                opcode = PUSH_ZO_SS;
                                break;
                            case ES:
                                opcode = PUSH_ZO_ES;
                                break;
                            case FS:
                                opcode = PUSH_ZO_FS;
                                break;
                            case GS:
                                opcode = PUSH_ZO_GS;
                                break;
                        }
                }
            }
    }

    if (opcode < 0) {
        printf("push %s not yet supported\n", arg);
        exit(EXIT_FAILURE);
    }

    get_instruction_operation_code(&operands, opcode);
    digits_to_hex_string(&operands, hex_string);
}

/******** Instruction: mov
 * - MOV r/m, r
 * - MOV r  , r/m
 * - MOV r  , imm
 * - MOV r/m, imm
 */

void
handle_mov(const struct Instruction* ins, char* hex_string)
{
    long mov_op_code, instruction_operation;
    struct OpData operands;

    init_opdata(&operands);
    get_mod_sib_imm_data(&operands, ins->argument[0], ins->argument[1]);

    mov_op_code = get_mov_opcode(&operands);
    get_instruction_operation_code(&operands, mov_op_code);
    digits_to_hex_string(&operands, hex_string);

    return;
}

/******** Instruction: nop
 * alias mnemonic for XCHG (E)AX, (E)AX instruction.
 */

void
handle_nop(const struct Instruction* ins, char* hex_string)
{
    memcpy(hex_string, "db 0x90\n", sizeof("db 0x90\n"));
    return;
}

/******** Instruction: Int imm8
 * - Destination operand specifies a vector from 0 to 255 inclusive.
 * - cd ib
 */

void
handle_int(const struct Instruction* ins, char* hex_string)
{
    struct OpData operands;

    init_opdata(&operands);

    operands.rm.rm_word_size = SZ_DISP8;
    get_immediate_values(&operands, ins->argument[0], SZ_DISP8);
    get_instruction_operation_code(&operands, 0xcd);
    digits_to_hex_string(&operands, hex_string);
}


void
parse_instruction_str(char* s, struct Instruction* ins, size_t len)
{
    size_t mnemonic_len, s_len, arg_len;
    char* saveptr;

    ins->mnemonic = strtok_r(s, " \t", &saveptr);

    mnemonic_len = strlen(s);
    clean_instruction_string(ins->mnemonic, mnemonic_len);

    if (mnemonic_len == len)
        return;

    s += mnemonic_len + 1;
    s_len = clean_instruction_string(s, len - mnemonic_len);

    arg_len = get_argument(ins->argument[0], s, s_len);
    arg_len += get_argument(ins->argument[1], &s[arg_len], s_len - arg_len);
    arg_len += get_argument(ins->argument[2], &s[arg_len], s_len - arg_len);
}


bool handle_instruction(char* s, size_t len, FILE* ostream)
{
    struct Instruction ins;
    bool return_value;

    ins.argument[0][0] = ins.argument[1][0] = ins.argument[2][0] = 0;
    ins.label = ins.mnemonic = NULL;
    if (len == 0)
        return true;

    if (s[len - 1] == ':') {
        ins.label = s;
        ins.label[len - 1] = '\0';
    }
    else
    parse_instruction_str(s, &ins, len);

    return_value = emit_intruction_code(&ins, ostream);
    return return_value;
}


size_t
clean_instruction_string(char* s, size_t len)
{
    size_t i;
    char* clean_str = (char*)malloc(len);
    if (clean_str == NULL) {
        perror("Critital heap error()");
        exit(EXIT_FAILURE);
    }

    for (size_t j, i = j = 0; i < len; i++) {
        if (s[i] == ' ' || s[i] == '\t')
            continue;
        if (isalpha(s[i]))
            clean_str[j++] = tolower(s[i]);
        else
            clean_str[j++] = s[i];
    }

    i = strlen(clean_str);
    memcpy(s, clean_str, i + 1);
    free(clean_str);

    return i;
}


size_t
get_argument(char* d, char* s, size_t len)
{
    size_t i = 0;
    if (s[i] == ',') // last arg was <mem>
        ++s;

    for (; s[i]; i++) {
        if (s[i] == ']') {
            d[i] = s[i];
            i++;
            d[i] = '\0';
            return i;
        }
        if (s[i] == ',')
            break;
        d[i] = s[i];
    }
    d[i] = '\0';
    return i + 1;
}


bool
emit_intruction_code(struct Instruction* ins, FILE* ostream)
{
    uint32_t n_bytes_read;
    char* instruction_code;
    void (*handle_mnemonic)(const struct Instruction* ins, char* hex_string);

    ins->first_pass_flag = (ostream == NULL);

    if (ins->label) {
        if (ins->first_pass_flag && hash_insert(ins->label) == false) {
            fprintf(stderr, "fatal: duplicate label %s\n", ins->label);
            exit(EXIT_FAILURE);
        }

        return true;
    }

    handle_mnemonic = lookup_handler(ins->mnemonic);
    if (handle_mnemonic == NULL) {
        printf("Critical mnemonic %s not implemented.\n", ins->mnemonic);
        exit(EXIT_FAILURE);
    }

    instruction_code = (char*)malloc(BIG_ENOUGH + 1);
    if (instruction_code == NULL) {
        perror("Critical heap error");
        exit(EXIT_FAILURE);
    }

    n_bytes_read = 0;
    handle_mnemonic(ins, instruction_code);
    write_instruction_code_to_file(ostream, instruction_code);

    n_bytes_read = count_instruction_bytes(instruction_code);
    if (ins->first_pass_flag == true)
        g_total_bytes += n_bytes_read;

    else
        g_current_byte_offset += n_bytes_read;

    free(instruction_code);

    return true;
}


size_t
count_instruction_bytes(const void* instruction_code)
{
    size_t n_read;
    const char* ip = (char*)instruction_code;

    n_read = 0;
    for (int i = 0, j = 1; ip[j]; i++, j++) {
        if (ip[i] == '0' && ip[j] == 'x')
            n_read++;
    }
    return n_read;
}


void
write_instruction_code_to_file(FILE* ostream, const char* string)
{
    size_t header_len;

    if (ostream != NULL) {
        header_len = strlen(string);
        if (fwrite(string, sizeof(char), header_len, ostream) != header_len) {
            perror("fwrite() error\n");
            exit(EXIT_FAILURE);
        }
    }
}


void
digits_to_hex_string(const struct OpData* op, char* hex_string)
{
    int i, j;
    int high_nibble, low_nibble;

    char hex_chars[] = {
        '0', '1', '2', '3', '4',
        '5', '6', '7', '8', '9',
        'a', 'b', 'c', 'd', 'e', 'f'
    };

    hex_string[0] = 'd';
    hex_string[1] = 'b';
    hex_string[2] = ' ';

    for (j = ARR_LIT_SZ - 1, i = 3; j >= 0; j--) {
        if (op->flags_array_literal[j] == true) {
            high_nibble = op->values_array_literal[j] & 0xf0;
            high_nibble >>= 4;

            low_nibble = op->values_array_literal[j] & 0x0f;

            hex_string[i++] = '0';
            hex_string[i++] = 'x';
            hex_string[i++] = hex_chars[high_nibble];
            hex_string[i++] = hex_chars[low_nibble];
            hex_string[i++] = ',';
        }
    }
    hex_string[i - 1] = '\n';
    hex_string[i] = '\0';

    return;
}
