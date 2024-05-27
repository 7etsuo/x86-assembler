/** x86 assembler
 * https://www.x.com/7etsuo
 */

#include "./include/assembler_perfect_hash.h"
#include "./include/assembler_registers.h"
#include "./include/assembler_sizes.h"

#include <stddef.h>
#include <stdio.h> 

const char* reg_32[] = { "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi", NULL };
const char* reg_16[] = { "ax", "cx", "dx", "bx", "sp", "bp", "si", "di", NULL };
const char* reg_8[] = { "al", "cl", "dl", "bl", "ah", "ch", "dh", "bh", NULL };

struct {
    int register_type;
    char* register_name;
    int size;
    int register_code;
} register_value_table[] = {
    { REG_GP,       "rax"       , -1        , -1  },
    { REG_GP,       "rbx"       , -1        , -1  },
    { REG_GP,       "rcx"       , -1        , -1  },
    { REG_GP,       "rdx"       , -1        , -1  },
    { REG_GP,       "rsi"       , -1        , -1  },
    { REG_GP,       "rdi"       , -1        , -1  },
    { REG_GP,       "rsp"       , -1        , -1  },
    { REG_GP,       "rbp"       , -1        , -1  },
    { REG_GP,       "r8"        , -1        , -1  },
    { REG_GP,       "r9"        , -1        , -1  },
    { REG_GP,       "r10"       , -1        , -1  },
    { REG_GP,       "r11"       , -1        , -1  },
    { REG_GP,       "r12"       , -1        , -1  },
    { REG_GP,       "r13"       , -1        , -1  },
    { REG_GP,       "r14"       , -1        , -1  },
    { REG_GP,       "r15"       , -1        , -1  },
    { REG_GP,       "eax"       , SZ_DISP32    , EAX },
    { REG_GP,       "ebx"       , SZ_DISP32    , EBX },
    { REG_GP,       "ecx"       , SZ_DISP32    , ECX },
    { REG_GP,       "edx"       , SZ_DISP32    , EDX },
    { REG_GP,       "esi"       , SZ_DISP32    , ESI },
    { REG_GP,       "edi"       , SZ_DISP32    , EDI },
    { REG_GP,       "esp"       , SZ_DISP32    , ESP },
    { REG_GP,       "ebp"       , SZ_DISP32    , EBP },
    { REG_GP,       "r8d"       , -1        , -1  },
    { REG_GP,       "r9d"       , -1        , -1  },
    { REG_GP,       "r10d"      , -1        , -1  },
    { REG_GP,       "r11d"      , -1        , -1  },
    { REG_GP,       "r14d"      , -1        , -1  },
    { REG_GP,       "r15d"      , -1        , -1  },
    { REG_GP,       "ax"        , SZ_DISP16    , AX  },
    { REG_GP,       "bx"        , SZ_DISP16    , CX  },
    { REG_GP,       "cx"        , SZ_DISP16    , CX  },
    { REG_GP,       "dx"        , SZ_DISP16    , DX  },
    { REG_GP,       "si"        , SZ_DISP16    , SI  },
    { REG_GP,       "di"        , SZ_DISP16    , DI  },
    { REG_GP,       "sp"        , SZ_DISP16    , SP  },
    { REG_GP,       "bp"        , SZ_DISP16    , BP  },
    { REG_GP,       "r8w"       , -1        , -1  },
    { REG_GP,       "r9w"       , -1        , -1  },
    { REG_GP,       "r10w"      , -1        , -1  },
    { REG_GP,       "r11w"      , -1        , -1  },
    { REG_GP,       "r12w"      , -1        , -1  },
    { REG_GP,       "r13w"      , -1        , -1  },
    { REG_GP,       "r14w"      , -1        , -1  },
    { REG_GP,       "r15w"      , -1        , -1  },
    { REG_GP,       "al"        , SZ_DISP8     , AL  },
    { REG_GP,       "cl"        , SZ_DISP8     , CL  },
    { REG_GP,       "dl"        , SZ_DISP8     , DL  },
    { REG_GP,       "bl"        , SZ_DISP8     , BL  },
    { REG_GP,       "ah"        , SZ_DISP8     , AH  },
    { REG_GP,       "ch"        , SZ_DISP8     , CH  },
    { REG_GP,       "dh"        , SZ_DISP8     , DH  },
    { REG_GP,       "bh"        , SZ_DISP8     , BH  },
    { REG_GP,       "r13b"      , -1        , -1  },
    { REG_POINTER,  "rip"       , -1        , -1  },
    { REG_POINTER,  "eip"       , SZ_DISP32    , -1  },
    { REG_POINTER,  "ip"        , SZ_DISP8     , -1  },
    { REG_SEGMENT,  "cs"        , SZ_DISP8     , CS  },
    { REG_SEGMENT,  "ds"        , SZ_DISP8     , DS  },
    { REG_SEGMENT,  "ss"        , SZ_DISP8     , SS  },
    { REG_SEGMENT,  "es"        , SZ_DISP8     , ES  },
    { REG_SEGMENT,  "fs"        , SZ_DISP8     , FS  },
    { REG_SEGMENT,  "gs"        , SZ_DISP8     , GS  },
    { -1,           "dwordptr"  , SZ_DISP32    , -1  },
    { -1,           "dword"     , SZ_DISP32    , -1  },
    { -1,           "wordptr"   , SZ_DISP16    , -1  },
    { -1,           "word"      , SZ_DISP16    , -1  },
    { -1,           "byteptr"   , SZ_DISP8     , -1  },
    { -1,           "byte"      , SZ_DISP8     , -1  },
};


char
get_register_code(const char* reg) // 3 bit register code
{
    int hash_value = register_has_key(reg);
    return hash_value == -1 ? hash_value : register_value_table[hash_value].register_code;
}


int
get_register_size(const char* reg)
{
    int hash_value = register_has_key(reg);
    return hash_value == -1 ? hash_value : register_value_table[hash_value].size;
}


int
get_register_type(const char* reg)
{
    int hash_value = register_has_key(reg);
    return hash_value == -1 ? hash_value : register_value_table[hash_value].register_type;
}
