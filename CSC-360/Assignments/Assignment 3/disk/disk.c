#include "disk.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 4096

void write_block(FILE* disk, size_t block_num, void* buffer, size_t buf_size) {
    fseek(disk, block_num * BLOCK_SIZE, SEEK_SET);
    fwrite(buffer, buf_size, 1, disk);
}

void read_block(FILE* disk, size_t block_num, void* buffer, size_t buf_size) {
    fseek(disk, block_num * BLOCK_SIZE, SEEK_SET);
    fread(buffer, buf_size, 1, disk);
}

void clear_disk(FILE* disk) {
    ftruncate(fileno(disk), BLOCK_SIZE * NUM_BLOCKS);

    void* buffer = malloc(BLOCK_SIZE);
    memset(buffer, 0, BLOCK_SIZE);

    for (size_t i = 0; i < NUM_BLOCKS; ++i) {
        write_block(disk, i, buffer, BLOCK_SIZE);
    }
    free(buffer);
}

