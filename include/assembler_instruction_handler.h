/** x86 assembler
 * https://www.x.com/7etsuo
 */

#ifndef ASSEMBLER_INSTRUCTION_HANDLER_H
#define ASSEMBLER_INSTRUCTION_HANDLER_H

#include <stddef.h>                                 // NULL 
#include <stdbool.h>
#include <stdio.h>                                  // FILE*
#include "assembler_sizes.h"                        // MAX_OPS, MAX_READ

struct Instruction {
    char* label;                                    // an identifier which is followed by a colon
    char* mnemonic;                                 // reserved name for a class of instruction opcodes which have the same function
    char argument[MAX_OPERANDS][MAX_READ_LINE]; // optional, either literals, identifiers, or data
    bool first_pass_flag;
};

void
handle_sysmov(const struct Instruction* ins, char* hex_string);
void
handle_push(const struct Instruction* ins, char* hex_string);
void
handle_xor(const struct Instruction* ins, char* hex_string);
void
handle_mov(const struct Instruction* ins, char* hex_string);
void
handle_nop(const struct Instruction* ins, char* hex_string);
void
handle_int(const struct Instruction* ins, char* hex_string);
void
handle_jmp(const struct Instruction* ins, char* hex_string);

struct OpData;                                      // forward declaration


void
(*lookup_handler(const char* mnemonic))(const struct Instruction* ins, char* hex_string);
void
parse_instruction_str(char* s, struct Instruction* ins, size_t len);
void
write_instruction_code_to_file(FILE* ostream, const char* string);
void
digits_to_hex_string(const struct OpData* op, char* hex_string);
bool
emit_intruction_code(struct Instruction* ins, FILE* ostream);
size_t
count_instruction_bytes(const void* instruction_code);
bool
handle_instruction(char* s, size_t len, FILE* ostream);
size_t
clean_instruction_string(char* s, size_t len);
size_t
get_argument(char* d, char* s, size_t len);

#endif // ASSEMBLER_INSTRUCTION_HANDLER_H
