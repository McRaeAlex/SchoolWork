#ifndef DISK_HEADER
#define DISK_HEADER
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

void write_block(FILE* disk, size_t block_num, void* buffer, size_t buf_size);

void read_block(FILE* disk, size_t block_num, void* buffer, size_t buf_size);

void clear_disk(FILE* disk);

#endif
