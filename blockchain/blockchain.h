#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <stdint.h>
#include <time.h>

#define MAX_DATA_SIZE 256
#define HASH_SIZE 65  /* 64 characters + null terminator */
typedef struct Block {
    int index;
    time_t timestamp;
    char data[MAX_DATA_SIZE];
    char prev_hash[HASH_SIZE];
    char hash[HASH_SIZE];
    int nonce;
} Block;
/* Function declarations */
Block create_genesis_block();
Block create_new_block(Block prev_block, const char *data);
void calculate_hash(Block *block);
int proof_of_work(Block *block, int difficulty);
void save_block(Block block, const char *filename);
Block get_last_block(const char *filename);

#endif
