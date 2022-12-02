#include "common.h"
#include "days.h"

// macro for checkfing days and printing if error happens
#define CHECK_DAY(e) \
    if(e()) \
        fprintf(stderr, "[!] Error running '"#e"'\n");


int main(void) {
    printf("[*] Hello Advent!\n");

    CHECK_DAY(day01);
    CHECK_DAY(day02);

    return EXIT_SUCCESS;
}