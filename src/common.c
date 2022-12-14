#include "common.h"


char* read_file_to_string(char* path) {
    FILE* f = fopen(path, "r");
    if(f == NULL) {
        fprintf(stderr, "[!] Couldn't open file '%s'\n", path);
        exit(EXIT_FAILURE);
    }

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


char* copy_str(char* s) {
    // Length of the string + 1 for '\0'
    size_t len = strlen(s) + 1;

    char* new_s = malloc(len);
    strncpy(new_s, s, len);

    return new_s;
}

int compare_u32(const void* in_a, const void* in_b) {
    uint32_t a = *((uint32_t*)in_a);
    uint32_t b = *((uint32_t*)in_b);
    if(a == b) return 0;
    else if(a < b) return -1;
    else return 1;
}

int compare_sizet(const void* in_a, const void* in_b) {
    size_t a = *((size_t*)in_a);
    size_t b = *((size_t*)in_b);
    if(a == b) return 0;
    else if(a < b) return -1;
    else return 1;
}

size_t count_lines(const char* s) {
    size_t len = strlen(s);
    size_t newlines = 1;
    for(size_t i = 0; i < len; i++) {
        if(s[i] == '\n')
            newlines += 1;
    }

    return newlines;
}


#ifndef __USE_MISC
char* strsep(char** sp, char* sep) {
    char* p;
    char* s;
    if (sp == NULL || *sp == NULL || **sp == '\0') return(NULL);
    s = *sp;
    p = s + strcspn(s, sep);
    if (*p != '\0') *p++ = '\0';
    *sp = p;
    return s;
}
#endif
