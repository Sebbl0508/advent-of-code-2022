#include "common.h"

int compare_u32(const void* in_a, const void* in_b) {
    uint32_t a = *((uint32_t*)in_a);
    uint32_t b = *((uint32_t*)in_b);
    if(a == b) return 0;
    else if(a < b) return -1;
    else return 1;
}

bool pt1(char** f);

bool day01() {
    char* file = read_file_to_string("./inputs/day01.txt");

    char* file_orig = file;
    pt1(&file);
    free(file_orig);


    printf("\n");
    return EXIT_SUCCESS;
}

bool pt1(char** f) {
    char* line = strsep(f, "\n");

    size_t totals_cnt = 0;
    uint32_t* totals = NULL;
    uint32_t cnt = 0;

    while(line != NULL) {
        // Check if the line is blank
        if(strcmp(line, "") == 0) {
            // Next elf
            totals_cnt += 1;
            totals = reallocarray(totals, totals_cnt, sizeof(uint32_t));
            totals[totals_cnt-1] = cnt;
            cnt = 0;
        }

        // count up by the number in the line
        cnt += atoi(line);

        // split string further
        line = strsep(f, "\n");
    }
    qsort(totals, totals_cnt, sizeof(uint32_t), compare_u32);
    printf("[*] (D01-1) %d\n", totals[totals_cnt-1]);

    printf(
        "[*] (D01-2) %d\n",
        totals[totals_cnt-1] + 
        totals[totals_cnt-2] +
        totals[totals_cnt-3]
    );

    return EXIT_SUCCESS;
}