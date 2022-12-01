#include "common.h"


char* read_file_to_string(char* path) {
    FILE* f = fopen(path, "r");

    // seek to the end of the file to
    // figure out the length and then rewind it
    // back to the beginning
    fseek(f, 0, SEEK_END);
    size_t filesize = (size_t)ftell(f);
    rewind(f);

    // allocate the memory
    // allocate 1 byte more for '\0' at the end of the string
    char* buf = malloc(filesize + 1);

    // read the file's contents into `buf`
    size_t bytes_read = fread(buf, sizeof(char), filesize, f);

    if(filesize != bytes_read) {
        fprintf(stderr, "[!] Error reading file '%s', filesize is %ld and %ld were read\n", path, filesize, bytes_read);
        exit(EXIT_FAILURE);
    }

    buf[bytes_read] = '\0';

    fclose(f);
    return buf;
}