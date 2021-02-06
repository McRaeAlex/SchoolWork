#include "../io/File.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    const char str1[] =
        "Hello this is a long string which I will use for testing\n";
    const size_t size = strlen(str1) * sizeof(char);

    InitLLFS();

    // Create and set the file
    file_o *f1 = file_open("Hello.txt");
    file_seek(f1, 0, TWC_SEEK_START);
    file_write(f1, str1, size, 1);

    file_close(f1);

    // Open the file again and read it into a buffer
    char *buffer = malloc(size);

    file_o *f2 = file_open("Hello.txt");
    file_seek(f2, 0, TWC_SEEK_START);
    file_read(f2, buffer, size, 1);

    int res = strcmp(buffer, str1);
    if (res == 0) {
        return 0;
    } else {
        return -1;
    }
}
