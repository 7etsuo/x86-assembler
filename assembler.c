/** x86 assembler
 * https://www.x.com/7etsuo
 */

#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "./include/assembler_instruction_handler.h"
#include "./include/assembler_sizes.h"
#include "./include/assembler_cuckoo.h"

const char* g_outfile = "outfile.asm";
const char* g_infile = "infile.asm";

const char* g_out_start =
    "; x86 assembler\n"
    "; https://www.x.com/7etsuo \n"
    ";\n"
    "; nasm -f elf -o outfile.o outfile.asm\n"
    "; ld -m elf_i386 outfile.o -o a.out \n"
    "\n"
    "[BITS 32]\n"
    "global _start\n"
    "_start:\n";

bool
read_loop(FILE* ostream, FILE* istream);

int
main(int argc, char* argv[])
{
    FILE* ostream, * istream;
    hash_init();

    istream = fopen(g_infile, "r");
    if (istream == NULL) {
        perror("fopen()");
        return 1;
    }

    if (read_loop(NULL, istream) == false) // label pass
        goto error_state;
    rewind(istream); // be kind

    ostream = fopen(g_outfile, "w");
    if (ostream == NULL) {
        fclose(istream);
        perror("fopen() ostream");
        return 1;
    }

    write_instruction_code_to_file(ostream, g_out_start); // write header to ostream
    if (read_loop(ostream, istream) == false)             // assemble pass
        goto error_state;

    if (ferror(istream))
        goto error_state;

    fclose(ostream);
    fclose(istream);
    hash_free();

    return 0;

error_state:
    hash_free();
    perror("file error (line len to small on errno success)");
    exit(EXIT_FAILURE);
}

bool
read_loop(FILE* ostream, FILE* istream)
{
    char line[MAX_READ_LINE];
    size_t len, i;

    while (fgets(line, MAX_READ_LINE, istream)) {
        len = strcspn(line, "\n");
        if (len == MAX_READ_LINE - 1) {
            fclose(istream);
            return false;
        }
        line[len] = '\0'; // discard '\n'

        for (i = 0; i < len; i++) { // strip white space
            if (line[i] == ' ' || line[i] == '\t')
                continue;
            break;
        }
        len = len - i;
        if (len == 0)
            continue;
        if (!handle_instruction(&line[i], len, ostream))
            break;
    }
    return true;
}

