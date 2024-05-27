
#include <string.h>
#include "register-code.h"

static int T1[] = { 6, 13, 27 };

static int T2[] = { 23, 30, 7 };

static int G[] = { 0, 0, 0, 0, 0, 0, 2, 31, 27, 23, 13, 0, 16, 4, 18, 0,
  0, 0, 30, 12, 31, 33, 3, 0, 8, 30, 20, 0, 6, 9, 22, 6, 10, 21, 7, 14 };

static char* K[] = { "eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi",
  "ax", "cx", "dx", "bx", "sp", "bp", "si", "di", "al", "cl", "dl", "bl",
  "ah", "ch", "dh", "bh" };

static int register_hash_g(const char* key, const int* T)
{
    int i, sum = 0;

    for (i = 0; key[i] != '\0'; i++) {
        sum += T[i] * key[i];
        sum %= 36;
    }
    return G[sum];
}

static int register_perfect_hash(const char* key)
{
    if (strlen(key) > 3)
        return 0;

    return (register_hash_g(key, T1) + register_hash_g(key, T2)) % 36;
}

int register_has_key(const char* reg)
{
    int hash_value = register_perfect_hash(reg);

    if (hash_value < 24 && strcmp(reg, K[hash_value]) == 0)
        return hash_value;

    return -1;
}
