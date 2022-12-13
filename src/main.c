#include "common.h"
#include "days.h"

// macro for checkfing days and printing if error happens
#define CHECK_DAY(e) \
    if(e()) \
        fprintf(stderr, "[!] Error running '"#e"'\n");


int main(void) {
    printf("[*] Hello Advent!\n");

    /*
    CHECK_DAY(day01);
    CHECK_DAY(day02);
    CHECK_DAY(day03);
    CHECK_DAY(day04);
    CHECK_DAY(day05);
    CHECK_DAY(day06);
    CHECK_DAY(day07);
    CHECK_DAY(day08);
    CHECK_DAY(day09);
    CHECK_DAY(day10);
    CHECK_DAY(day11);
    */
    CHECK_DAY(day12);

    return EXIT_SUCCESS;
}
