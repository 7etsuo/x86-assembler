/* ****** cuckoo hash
/* x86 assembler
 * https://www.x.com/7etsuo
 *
 *
 *
 * usage example:
 *   int main(int argc, char** argv)
 *   {
 *   HTENTRY* htptr;
 *
 *   hash_init( );
 *   hash_insert("this is a test");
 *   hash_insert("this is another test");
 *
 *   htptr = hash_lookup("this is a test");
 *   if (htptr != NULL)
 *   {
 *      puts(htptr->value);
 *   }
 *
 *   hash_print( );
 *   hash_free( );
 *
 *   return 0;
 *   }
 */

#ifndef ASSEMBLER_CUCKOO_H
#define ASSEMBLER_CUCKOO_H

#include <stdint.h> 
#include <stdbool.h> 

#define HASH_MAX_HASH_FUNCTIONS 3
#define HASH_MAX_HT_SIZE        5
#define HASH_MAX_KEY_LEN        100

typedef struct {
    uint32_t key;
    uint32_t byte_num;
    char value[HASH_MAX_KEY_LEN];
} HTENTRY;

extern uint32_t g_total_bytes;

void
hash_init(void);
void
hash_free(void);
HTENTRY*
hash_lookup(const void* key);
bool
hash_insert(const void* entry);

#endif // ASSEMBLER_CUCKOO_H
