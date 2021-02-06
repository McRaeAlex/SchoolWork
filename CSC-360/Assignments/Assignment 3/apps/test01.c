#include "../io/File.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    char str[] = "Hello World! ";
    char str2[] = "This is alex!";
    char str3[] = "This is the list file";

    InitLLFS();

    file_o *f = file_open("test_1.txt");

    file_write(f, str, (strlen(str)) * sizeof(char), 1);
    file_write(f, str2, (strlen(str2)) * sizeof(char), 1);

    file_o *f2 = file_open("test_2.txt");

    file_write(f2, str2, strlen(str2) * sizeof(char), 1);
    file_seek(f2, 0, TWC_SEEK_START);
    file_write(f2, str, strlen(str) * sizeof(char), 1);

    dir_make("test_dir");
    dir_change("test_dir");

    file_o *f3 = file_open("list.txt");

    file_write(f3, str3, strlen(str3) * sizeof(char), 1);

    file_close(f3);

    file_close(f2);

    file_close(f);

    return 0;
}
