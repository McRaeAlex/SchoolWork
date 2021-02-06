#ifndef FILE_HEADER
#define FILE_HEADER

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* Ideas:
 * Some things we need are a way to create a file. A way to delete a file. A way to read a file, keeping track of location. A way to seek relative to current location in the file.
 *
 */

typedef struct file_o {
    // the inode index
    uint8_t inode_index; 
    // the cursor into the file where 0 is the start of the file
    uint32_t cursor;
} file_o;

void InitLLFS();

// TWCOpen opens a file, creating it if it doesn't exist.
// The file must be closed using TWCClose.
// If for whatever reason opening the file fails, -1 will be returned and errno will be set.
file_o* file_open(const char* pathname);

// TWCClose closes a file and prevents any more file operations on the file.
// If TWCClose fails to close the file, false will be returned and errno will be set
bool file_close(file_o* file);

// TWCRemove removes a file pointed to by pathname
// Returns false if fails to remove and sets errno
bool file_remove(const char* pathname);

enum TWC_SEEK_OPTION_ENUM {
    TWC_SEEK_START,
    TWC_SEEK_CUR,
    TWC_SEEK_END
};

bool file_seek(file_o* file, long offset, enum TWC_SEEK_OPTION_ENUM option);

// TWCRead reads from the current position in the file for size * nmemb bytes into ptr.
// ptr is expect to be able to hold enough the data. TODO: maybe change this
// return -> the number of items read. Not the number of bytes read.
size_t file_read(file_o* file, void* ptr, size_t size, size_t nmemb);

// TWCWrite reads write to the current positiion in the file extending the file if needed. Returns the number of items written.
size_t file_write(file_o* file, const void* ptr, size_t size, size_t nmemb);

void dir_make(const char* name);
bool dir_change(const char* path);

#endif
