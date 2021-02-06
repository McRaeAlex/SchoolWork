#include "File.h"
#include "./../disk/disk.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structures

// inode_o is representation of a inode that is 32 bytes long
typedef struct inode_o {
    uint32_t length;
    uint32_t type; // 0 is directory, 1 is file
    uint16_t block_numbers[10];
    uint16_t block_number_single;
    uint16_t block_number_double;
} inode_o;

typedef struct inode_block {
    inode_o inodes[16];
} inode_block;

typedef struct superblock_o {
    int32_t magic_number;
    int32_t num_blocks;
    int32_t num_inodes;
    uint8_t
        free_inodes[16]; // A array of inodes where 0 means taken 1 means free
} superblock_o;

typedef struct freeblock_o {
    uint8_t blocks[512]; // This is a array of blocks were 1 means free 0 means
                         // taken
} freeblock_o;

typedef struct directory_o {
    char key[32];
    uint8_t inode;
} directory_o;

// Forward declares

file_o *inode_create(const char *pathname, uint32_t type);

size_t claim_block_index();

// inode_claim_index claims a index in the inode blocks for a inode flipping the
// bit in the super block
size_t inode_claim_index();

// inode_calc_block_index_from_index converts a inode index into its block index
// ie 0 -> 2nd block
size_t inode_calc_block_index_from_index(size_t index);

void dir_add_map(size_t dir_inode_index, const char *pathname,
                 size_t inode_index);

size_t dir_get_inode_index(size_t dir, const char *name);

// Globals

static FILE *disk;
static superblock_o superblock;
static size_t cwd_inode_index; // This is the index of the inode that is the
                               // current working directory

/*static map<files> open_files; // This is a mapp of all the open file*/

void InitLLFS() {
    disk = fopen("vdisk", "w+");
    clear_disk(disk);

    // Write the super block
    superblock.magic_number = 42;
    superblock.num_blocks = 512;
    superblock.num_inodes = 128;
    memset(superblock.free_inodes, 0xFF, sizeof(superblock.free_inodes));
    write_block(disk, 0, &superblock, sizeof(superblock));

    // Write the free block
    freeblock_o freeblock = {0};
    memset(&freeblock.blocks, 0xFF, 512); // set all the blocks to free
    freeblock.blocks[0] = 0x00;
    freeblock.blocks[1] = 0x3F;

    write_block(disk, 1, &freeblock.blocks, sizeof(freeblock.blocks));

    // Write the root inode out
    inode_o root = {0};
    root.length = 0;
    root.type = 0; // Directory
    memset(&root.block_numbers, 0, sizeof(root.block_numbers));

    inode_block first_block = {0};
    memset(&first_block, 0, sizeof(first_block));
    first_block.inodes[0] = root;
    write_block(disk, 2, &first_block, sizeof(first_block));
    superblock.free_inodes[0] = 0x7F;
    cwd_inode_index = 0;
    dir_add_map(cwd_inode_index, ".", 0);
    dir_add_map(cwd_inode_index, "..", 0);
}

file_o *file_open(const char *pathname) {
    size_t inode_index = dir_get_inode_index(cwd_inode_index, pathname);
    if (inode_index == -1) {
        return inode_create(pathname, 1);
    } else {
        file_o *f = malloc(sizeof(file_o));
        f->cursor = 0;
        f->inode_index = inode_index;
        return f;
    }
}

bool file_seek(file_o *file, long offset, enum TWC_SEEK_OPTION_ENUM option) {
    uint32_t curs = 0;

    // Get that inode block
    inode_block block = {0};
    size_t inode_block_index =
        inode_calc_block_index_from_index(file->inode_index);
    read_block(disk, inode_block_index, &block.inodes, sizeof(block.inodes));

    size_t inode_index_in_block = file->inode_index % 16;
    inode_o *inode = &block.inodes[inode_index_in_block];

    switch (option) {
    case TWC_SEEK_START:
        curs = 0;
        break;

    case TWC_SEEK_END:
        curs = inode->length;
        break;

    case TWC_SEEK_CUR:
        curs = file->cursor;
        break;
    }

    curs += offset;
    if (curs > inode->length || curs < 0) {
        fprintf(stderr, "Invalid cursor position");
        return false;
    }

    file->cursor = curs;

    return true;
}

bool file_close(file_o *file) {
    // flush the buffer (if we choose to implement one)
    // delete the data from the map of open files
    free(file);
    return false;
}

size_t file_write(file_o *file, const void *ptr, size_t size, size_t nmemb) {
    size_t ptr_size = size * nmemb;
    // get the file we want to write too
    size_t inode_block_index =
        inode_calc_block_index_from_index(file->inode_index);
    size_t inode_index_in_block = file->inode_index % 16;

    inode_block block = {0};
    read_block(disk, inode_block_index, &block, sizeof(block));

    inode_o *inode = &block.inodes[inode_index_in_block];
    if (file->cursor > inode->length) {
        fprintf(stderr, "Invalid file object. Cursor is past end of file\n");
        exit(-1);
    }

    // calc the first index into blocks and last
    size_t first = file->cursor / 512;
    size_t last = (file->cursor + ptr_size) / 512;
    if (last >= 9) {
        printf("File is too large\n");
        exit(-1);
    }

    size_t offset_into_ptr = 0; // this is in bytes

    // Handle writing to the first block
    {
        size_t bytes_into_first_block_taken = file->cursor % 512;
        size_t remaining_space_in_block = 512 - bytes_into_first_block_taken;
        size_t amount_to_write = (remaining_space_in_block < ptr_size)
                                     ? remaining_space_in_block
                                     : ptr_size;

        size_t block_index = inode->block_numbers[first];
        if (block_index < 9) {
            block_index = claim_block_index();
            inode->block_numbers[first] = block_index;
        }

        uint8_t *first_block = (uint8_t *)malloc(512);
        read_block(disk, block_index, first_block, 512);
        memcpy(first_block + bytes_into_first_block_taken, ptr,
               amount_to_write);
        offset_into_ptr += amount_to_write;
        write_block(disk, block_index, first_block, 512);
        free(first_block);
    }

    // Go through each remaining block and write the data into it
    for (size_t i = first + 1; i < last; i++) {
        size_t block_index = inode->block_numbers[i];
        // if block index is less than 9 then allocate it
        if (block_index < 9) {
            block_index = claim_block_index();
            inode->block_numbers[i] = block_index;
        }

        // write a portion of the data stored in ptr into the block
        size_t remaining = ptr_size - offset_into_ptr;
        size_t amount_to_write = (512 < remaining) ? 512 : remaining;

        uint8_t *block = (uint8_t *)malloc(512);
        memcpy(block, ((uint8_t *)ptr) + offset_into_ptr, amount_to_write);
        offset_into_ptr += amount_to_write;
        write_block(disk, block_index, block, 512);
        free(block);
    }

    // Handle the last block
    size_t remaining = ptr_size - offset_into_ptr;
    if (remaining != 0) {
        size_t block_index = inode->block_numbers[last];
        // if block index is less than 9 then allocate it
        if (block_index < 9) {
            block_index = claim_block_index();
            inode->block_numbers[last] = block_index;
        }

        // write a portion of the data stored in ptr into the block
        size_t remaining = ptr_size - offset_into_ptr;
        size_t amount_to_write = remaining;

        uint8_t *block = (uint8_t *)malloc(512);
        memcpy(block, ((uint8_t *)ptr) + offset_into_ptr, amount_to_write);
        offset_into_ptr += amount_to_write;
        write_block(disk, block_index, block, 512);
        free(block);
    }

    inode->length += (inode->length < (file->cursor + ptr_size))
                         ? (file->cursor + ptr_size)
                         : inode->length;
    file->cursor += ptr_size;

    write_block(disk, inode_block_index, &block, sizeof(block));
    return nmemb;
}

size_t file_read(file_o *file, void *ptr, size_t size, size_t nmemb) {
    // read in inode for the file
    size_t block_index = inode_calc_block_index_from_index(file->inode_index);
    inode_block inodes = {0};
    read_block(disk, block_index, &inodes.inodes, sizeof(inodes.inodes));

    inode_o *file_i = &inodes.inodes[file->inode_index % 512];

    // get the first and last blocks from file->cursor and size * nmemb

    size_t fir_block_index = file->cursor / 512;
    size_t last_block_index = (file->cursor + size * nmemb) / 512;

    uint8_t amount_written = 0;

    // handle reading in the first block
    size_t fir_offset = file->cursor % 512;
    size_t read_amount = 512 - fir_offset;
    uint8_t buf[512] = {0};
    read_block(disk, file_i->block_numbers[fir_block_index], buf, 512);

    memcpy(ptr, buf + fir_offset, read_amount);
    amount_written += read_amount;

    // handle reading in the next blocks
    for (size_t i = fir_block_index + 1; i < last_block_index; i++) {
        read_block(disk, file_i->block_numbers[i],
                   ((uint8_t *)ptr) + amount_written, 512);
        amount_written += 512;
    }

    // handle reading in the last block
    if (fir_block_index != last_block_index) {
        read_block(disk, file_i->block_numbers[last_block_index],
                   ((uint8_t *)ptr) + amount_written,
                   (size * nmemb) - amount_written);
    }

    return nmemb;
}

// Freeblock stuff

size_t claim_block_index() {
    // get the free block
    freeblock_o free_block = {0};
    read_block(disk, 1, &free_block.blocks, sizeof(free_block.blocks));

    // go through the free block and find the first index that is free
    for (size_t i = 0; i < 512; ++i) {
        // go through each bit;
        uint8_t mask = 0x80;
        uint8_t val = free_block.blocks[i];

        for (size_t j = 0; j < 8; ++j) {
            if (val & mask) { // if this is a free block
                // Flip the bit
                free_block.blocks[i] = free_block.blocks[i] ^ mask;
                // Presist it to the disk
                write_block(disk, 1, &free_block.blocks,
                            sizeof(free_block.blocks));
                // Return the index
                return (i * 8) + j;
            }

            mask = (mask >> 1);
        }
    }

    printf("No free blocks\n");
    exit(-1);
}

// Inode stuff
// ------------
file_o *inode_create(const char *pathname, uint32_t type) {
    // Creates a new empty file inode
    inode_o new_file = {0};
    new_file.type = type;
    new_file.length = 0;
    memset(&new_file.block_numbers, 0, sizeof(new_file.block_numbers));

    // Gets a index for that inode to be stored at
    size_t inode_index = inode_claim_index();

    // Get that inode block
    inode_block block = {0};
    size_t inode_block_index = inode_calc_block_index_from_index(inode_index);
    read_block(disk, inode_block_index, &block.inodes, sizeof(block.inodes));

    size_t inode_index_in_block = inode_index % 16;
    block.inodes[inode_index_in_block] = new_file;

    write_block(disk, inode_block_index, &block.inodes, sizeof(block.inodes));

    // add the file to the current working directory
    dir_add_map(cwd_inode_index, pathname, inode_index);

    file_o *new_file_obj = (file_o *)malloc(sizeof(file_o));
    new_file_obj->inode_index = inode_index;
    new_file_obj->cursor = 0;
    return new_file_obj;
}

size_t inode_claim_index() {
    // go through the inode section of the super block and select the index
    for (size_t i = 0; i < 16; ++i) {
        // go through each bit;
        uint8_t mask = 0x80;
        uint8_t val = superblock.free_inodes[i];

        for (size_t j = 0; j < 8; ++j) {
            if (val & mask) {
                superblock.free_inodes[i] ^= mask;
                return (i * 8) + j;
            }

            mask = (mask >> 1);
        }
    }
    printf("Out of Inodes\n");
    exit(-1);
}

size_t inode_calc_block_index_from_index(size_t index) {
    return index / 16 + 2;
}

// Directory stuff
// ---------------
void dir_make(const char *name) {
    file_o *dir = inode_create(name, 0);
    dir_add_map(dir->inode_index, ".", dir->inode_index);
    dir_add_map(dir->inode_index, "..", cwd_inode_index);
    free(dir);
}

bool dir_change(const char *path) {
    // Read in cwd inode
    inode_block cwd_block = {0};
    size_t cwd_block_index = inode_calc_block_index_from_index(
        cwd_inode_index); // This is the block the inode is stored
    size_t cwd_in_block_index = cwd_inode_index % 16;

    read_block(disk, cwd_block_index, &cwd_block.inodes,
               sizeof(cwd_block.inodes));

    inode_o *cwd = &cwd_block.inodes[cwd_in_block_index];

    // get each block in the cwd file into a array of directory_o's
    size_t last = cwd->length / 512;
    size_t num_maps =
        cwd->length /
        sizeof(directory_o); // the number of file and directories in our cwd
    directory_o *dir_objs = malloc(
        num_maps * sizeof(directory_o)); // we need to get the number of file

    for (size_t i = 0; i <= last; ++i) {
        uint8_t *buffer = malloc(512);
        read_block(disk, cwd->block_numbers[i], buffer, 512);
        // put into the dir_objs
        uint8_t *ptr_into_dir_objs = ((uint8_t *)dir_objs) + (512 * i);
        size_t left_to_write = (cwd->length - (512 * i));
        size_t amount_to_write = left_to_write < 512 ? left_to_write : 512;
        memcpy(ptr_into_dir_objs, buffer, amount_to_write);
        free(buffer);
    }

    // check if path is avaliable
    for (size_t i = 0; i < num_maps; ++i) {
        directory_o obj = dir_objs[i];
        int res = strcmp(obj.key, path);
        if (res == 0) {
            free(dir_objs);
            cwd_block_index = obj.inode;
            return true;
        }
    }

    return false;
}

void dir_list() {
    // Read in cwd inode
    inode_block cwd_block = {0};
    size_t cwd_block_index = inode_calc_block_index_from_index(
        cwd_inode_index); // This is the block the inode is stored
    size_t cwd_in_block_index = cwd_inode_index % 16;

    read_block(disk, cwd_block_index, &cwd_block.inodes,
               sizeof(cwd_block.inodes));

    inode_o *cwd = &cwd_block.inodes[cwd_in_block_index];

    // get each block in the cwd file into a array of directory_o's
    size_t last = cwd->length / 512;
    size_t num_maps =
        cwd->length /
        sizeof(directory_o); // the number of file and directories in our cwd
    directory_o *dir_objs = malloc(
        num_maps * sizeof(directory_o)); // we need to get the number of file

    for (size_t i = 0; i <= last; ++i) {
        uint8_t *buffer = malloc(512);
        read_block(disk, cwd->block_numbers[i], buffer, 512);
        // put into the dir_objs
        uint8_t *ptr_into_dir_objs = ((uint8_t *)dir_objs) + (512 * i);
        size_t left_to_write = (cwd->length - (512 * i));
        size_t amount_to_write = left_to_write < 512 ? left_to_write : 512;
        memcpy(ptr_into_dir_objs, buffer, amount_to_write);
        free(buffer);
    }

    // check if path is avaliable
    for (size_t i = 0; i < num_maps; ++i) {
        directory_o obj = dir_objs[i];
        printf("NAME: %s, INODE INDEX: %d\n", obj.key, obj.inode);
    }
}

void dir_path() {
    // char *string = "";
    size_t inode_index =
        cwd_inode_index; // this is the inode we are currently looking at
    size_t last_inode =
        0; // this is the inode we look for to get the name of the last inode
    // bool first = true;
    while (inode_index != 0) {
        inode_block cwd_block = {0};
        size_t cwd_block_index = inode_calc_block_index_from_index(
            cwd_inode_index); // This is the block the inode is stored
        size_t cwd_in_block_index = cwd_inode_index % 16;

        read_block(disk, cwd_block_index, &cwd_block.inodes,
                   sizeof(cwd_block.inodes));

        inode_o *cwd = &cwd_block.inodes[cwd_in_block_index];

        // the number of file and directories in our cwd
        size_t num_maps = cwd->length / sizeof(directory_o);
        // we need to get the number of file
        directory_o *dir_objs = malloc(num_maps * sizeof(directory_o));

        size_t last = cwd->length / 512;
        for (size_t i = 0; i <= last; ++i) {
            uint8_t *buffer = malloc(512);
            read_block(disk, cwd->block_numbers[i], buffer, 512);
            // put into the dir_objs
            uint8_t *ptr_into_dir_objs = ((uint8_t *)dir_objs) + (512 * i);
            size_t left_to_write = (cwd->length - (512 * i));
            size_t amount_to_write = left_to_write < 512 ? left_to_write : 512;
            memcpy(ptr_into_dir_objs, buffer, amount_to_write);
            free(buffer);
        }

        /* TODO: we are having issues here. We need to get the name of the
         * directory of the one we came from */
        // check if path is avaliable
        bool found_parent = false;
        bool found_name = false;
        for (size_t i = 0; i < num_maps; ++i) {
            directory_o obj = dir_objs[i];
            if (strcmp("..", obj.key) == 0) {
                inode_index = obj.inode;
                found_parent = true;
            }

            if (obj.inode == last_inode) {
                // TODO: concat the string to the front of the list
                found_name = true;
            }

            if (found_name && found_parent) {
                break;
            }
        }

        free(dir_objs);
    }
}

size_t dir_get_inode_index(size_t dir, const char *name) {
    // Get the dir inode
    inode_block dir_block = {0};
    size_t dir_block_index = inode_calc_block_index_from_index(dir);

    read_block(disk, dir_block_index, &dir_block.inodes,
               sizeof(dir_block.inodes));

    inode_o *dir_i = &dir_block.inodes[dir % 16];

    // Read in the blocks
    size_t num_objs = dir_i->length / sizeof(directory_o);
    directory_o *objs = malloc(sizeof(directory_o) * num_objs);

    size_t remaining = dir_i->length;
    size_t last = dir_i->length / 512;
    for (size_t i = 0; i <= last; ++i) {
        size_t block_index = dir_i->block_numbers[i];

        uint8_t *offset = ((uint8_t *)objs) + (512 * i);

        size_t amount_to_write = remaining < 512 ? remaining : 512;

        read_block(disk, block_index, offset, amount_to_write);
    }

    size_t index = -1;
    for (size_t i = 0; i <= num_objs; ++i) {
        directory_o o = objs[i];
        if (strncmp(name, o.key, 32) == 0) {
            index = o.inode;
            break;
        }
    }
    free(objs);

    return index;
}

void dir_add_map(size_t dir_inode_index, const char *pathname,
                 size_t inode_index) {
    inode_block dir_block = {0};
    size_t dir_block_index = inode_calc_block_index_from_index(
        dir_inode_index); // This is the block the inode is stored
    size_t dir_in_block_index = dir_inode_index % 16;

    read_block(disk, dir_block_index, &dir_block.inodes,
               sizeof(dir_block.inodes));

    inode_o *dir = &dir_block.inodes[dir_in_block_index];

    // get the block we want from the length.
    size_t index_into_block_numbers = (dir->length + sizeof(directory_o)) / 512;

    size_t block_index = dir->block_numbers[index_into_block_numbers];

    if (block_index < 10) {
        block_index = claim_block_index();
        dir->block_numbers[index_into_block_numbers] = block_index;
    }

    size_t num_maps = 512 / sizeof(directory_o);
    directory_o* dir_objs = malloc(512); // a map of names to inode indices
    read_block(disk, block_index, dir_objs, 512);

    // go to the end of the file (ie. dir->length % 512) and add too it then
    // rewrite it.
    size_t index_into_dir_block = (dir->length % 512) / sizeof(directory_o);
    dir_objs[index_into_dir_block].inode = inode_index;
    strncpy(dir_objs[index_into_dir_block].key, pathname, 32);
    dir->length += sizeof(directory_o);

    write_block(disk, block_index, dir_objs, sizeof(directory_o) * num_maps);
    write_block(disk, dir_block_index, &dir_block, sizeof(dir_block));
    free(dir_objs);
}