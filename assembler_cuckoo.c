/** x86 assembler
 * https://www.x.com/7etsuo
 */

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h> 
#include <inttypes.h>

#include "./include/assembler_cuckoo.h"

HTENTRY* g_hashtable[HASH_MAX_HASH_FUNCTIONS][HASH_MAX_HT_SIZE];
uint32_t g_positions[HASH_MAX_HASH_FUNCTIONS];

uint32_t
hash_djb2(const void* data, size_t len)
{
    const char* p, * q;
    uint32_t hash = 5381;

    q = data + len;
    for (p = (char*)data; p < q; p++)
        hash = ((hash << 5) + hash) + *p;

    return hash;
}


uint32_t
hash_fnv1a(const void* data, size_t len)
{
    const char* p, * q;

    uint32_t hash = 0x01000193;
    uint32_t prime = 0x01000193;

    q = data + len;
    for (p = (char*)data; p < q; p++)
        hash = (*p ^ hash) * prime;

    return hash;
}


uint32_t
hash_crc32(const void* data, size_t len)
{
    static uint32_t table[256];
    static int have_table = 0;
    uint32_t rem, crc = 0L;
    uint8_t octet;
    int i, j;
    const char* p, * q;

    if (have_table == 0) {
        for (i = 0; i < 256; i++) {
            rem = i;
            for (j = 0; j < 8; j++) {
                if (rem & 1) {
                    rem >>= 1;
                    rem ^= 0xedb88320;
                }
                else
                rem >>= 1;
            }
            table[i] = rem;
        }
        have_table = 1;
    }

    crc = ~crc;
    q = data + len;
    for (p = (char*)data; p < q; p++) {
        octet = *p;
        crc = (crc >> 8) ^ table[(crc & 0xff) ^ octet];
    }
    return ~crc;
}


uint32_t
hash(const void* key, int function, size_t len)
{
    uint32_t hash; // for testing TODO: return in switch.

    switch (function) {
        case 1:
            hash = hash_djb2(key, len);
            break;
        case 2:
            hash = hash_fnv1a(key, len);
            break;
        case 3:
            hash = hash_crc32(key, len);
            break;
        default:
            fprintf(stderr, "fatal: invalid function number, hash()\n");
            exit(EXIT_FAILURE);
    }
    return (hash % HASH_MAX_HT_SIZE);
}


bool
hash_insert_priv(HTENTRY* entry, int id, int n_calls)
{
    size_t val_len;

    if (n_calls == HASH_MAX_HT_SIZE) {
        fprintf(stderr, "insert() fatal: implement rehash\n");
        exit(EXIT_FAILURE);
    }

    if (hash_lookup(entry->value))
        return false;

    val_len = strlen(entry->value);
    if (g_hashtable[id][g_positions[id]] != NULL) {
        puts("collision!");
        HTENTRY* displace = g_hashtable[id][g_positions[id]];
        g_hashtable[id][g_positions[id]] = entry;
        g_hashtable[id][g_positions[id]]->key = g_positions[id];

        hash_insert_priv(displace, (id + 1) % HASH_MAX_HASH_FUNCTIONS, n_calls + 1);
    }
    else {
        g_hashtable[id][g_positions[id]] = entry;
        g_hashtable[id][g_positions[id]]->key = g_positions[id];
    }

    return true;
}


bool
hash_insert(const void* entry)
{
    HTENTRY* ht_new;
    size_t instruction_len;
    const char* ep;

    ht_new = (HTENTRY*)malloc(sizeof(HTENTRY));
    if (ht_new == NULL) {
        goto insert_error;
    }

    ep = (char*)entry;
    instruction_len = strlen(ep);
    if (instruction_len + 1 > HASH_MAX_KEY_LEN) {
        goto insert_error;
    }

    memcpy(ht_new->value, ep, instruction_len + 1);
    ht_new->key = UINT32_MAX;
    if ((hash_insert_priv(ht_new, 0, 0)) == false)   // duplicate
    {
        free(ht_new);
        return false;
    }
    else {
        ht_new->byte_num = g_total_bytes;
    }

    return true;

insert_error:
    fprintf(stderr, "fatal error hash_insert()\n");
    exit(EXIT_FAILURE);
}


HTENTRY*
hash_lookup(const void* key)
{
    char* valp;
    size_t lenk;

    for (int i = 0; i < HASH_MAX_HASH_FUNCTIONS; i++) {
        lenk = strlen((char*)key);
        g_positions[i] = hash(key, i + 1, lenk);

        if (g_hashtable[i][g_positions[i]] == NULL)
            continue;

        valp = g_hashtable[i][g_positions[i]]->value;
        if (strncmp(valp, key, lenk + 1) == 0)
            return g_hashtable[i][g_positions[i]];
    }

    return NULL;
}


void
hash_print(void) // for testing.
{
    for (size_t i = 0; i < HASH_MAX_HASH_FUNCTIONS; i++) {
        for (size_t j = 0; j < HASH_MAX_HT_SIZE; j++) {
            if (g_hashtable[i][j] != NULL) {
                printf("entry:\t\t%s\nkey:\t\t%d\nlinenum:\t%d\n\n",
                       g_hashtable[i][j]->value,
                       g_hashtable[i][j]->key,
                       g_hashtable[i][j]->byte_num
                       );
            }
        }
    }
}


void
hash_init(void)
{
    g_total_bytes = 0;
    for (size_t i = 0; i < HASH_MAX_HASH_FUNCTIONS; i++) {
        for (size_t j = 0; j < HASH_MAX_HT_SIZE; j++) {
            g_hashtable[i][j] = NULL;
        }
    }
}


void
hash_delete(HTENTRY** pentry)
{
    free(*pentry);
    *pentry = NULL;
}


void
hash_free(void)
{
    for (size_t i = 0; i < HASH_MAX_HASH_FUNCTIONS; i++) {
        for (size_t j = 0; j < HASH_MAX_HT_SIZE; j++) {
            if (g_hashtable[i][j] != NULL) {
                hash_delete(&g_hashtable[i][j]);
            }
        }
    }
}
