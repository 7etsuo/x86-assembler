#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool write_start_to_file(FILE* ostream);
int main(int argc, char** argv)
{
    FILE* ostream = fopen("hex.asm", "w");
    if (ostream == NULL) {
        exit(EXIT_FAILURE);
    }

    write_start_to_file(ostream);
    fclose(ostream);
    return 0;
}

bool write_start_to_file(FILE* ostream)
{
    const char* header = "[BITS 32]\n"
        "global _start\n"
        "_start:\n";

    size_t header_len = strlen(header);
    if (fwrite(header, sizeof(char), header_len, ostream) != header_len) {
        perror("fwrite() error\n");
        return 0;
    }

    return 1;
}
