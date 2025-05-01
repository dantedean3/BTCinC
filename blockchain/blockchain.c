#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "blockchain.h"

extern void sha256(const char *str, char outputBuffer[65]);

Block create_genesis_block() {
    Block genesis;
    genesis.index = 0;
    genesis.timestamp = time(NULL);
    strcpy(genesis.data, "Genesis Block");
    strcpy(genesis.prev_hash, "0");
    genesis.nonce = 0;
    calculate_hash(&genesis);
    return genesis;
}

Block create_new_block(Block prev_block, const char *data) {
    Block new_block;
    new_block.index = prev_block.index + 1;
    new_block.timestamp = time(NULL);
    strncpy(new_block.data, data, MAX_DATA_SIZE);
    strcpy(new_block.prev_hash, prev_block.hash);
    new_block.nonce = 0;
    calculate_hash(&new_block);
    proof_of_work(&new_block, 1); /* Very low difficulty */
    return new_block;
}

void calculate_hash(Block *block) {
    char to_hash[1024];
    snprintf(to_hash, sizeof(to_hash), "%d%ld%s%s%d",
             block->index, block->timestamp,
             block->data, block->prev_hash, block->nonce);
    sha256(to_hash, block->hash);
}

int proof_of_work(Block *block, int difficulty) {
    char prefix[HASH_SIZE];
    memset(prefix, '0', difficulty);
    prefix[difficulty] = '\0';

    do {
        calculate_hash(block);
        block->nonce++;
    } while (strncmp(block->hash, prefix, difficulty) != 0);

    return block->nonce;
}

void save_block(Block block, const char *filename) {
    FILE *fp = fopen(filename, "a");
    if (!fp) {
        perror("Unable to open chain file");
        exit(1);
    }
    fprintf(fp, "%d|%ld|%s|%s|%s|%d\n",
            block.index, block.timestamp,
            block.data, block.prev_hash,
            block.hash, block.nonce);
    fclose(fp);
}

Block get_last_block(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        return create_genesis_block();
    }

    Block block;
    Block last_block = create_genesis_block();
    while (fscanf(fp, "%d|%ld|%255[^|]|%64[^|]|%64[^|]|%d\n",
                  &block.index, &block.timestamp, block.data,
                  block.prev_hash, block.hash, &block.nonce) == 6) {
        last_block = block;
    }
    fclose(fp);
    return last_block;
}

int main() {
    const char *filename = "chain.dat";
    Block last_block = get_last_block(filename);

    Block new_block = create_new_block(last_block, "Daily block");
    save_block(new_block, filename);

    printf("Added block #%d with hash %s\n", new_block.index, new_block.hash);
    return 0;
}
