#include "day_1.h"




static bool pt1_2(char** f);

bool day01(void) {
    char* file = read_file_to_string("./inputs/day01.txt");
    bool ret = EXIT_SUCCESS;

    char* file_orig = file;
    if(pt1_2(&file))
        ret = EXIT_FAILURE;

    free(file_orig);
    printf("\n");
    return ret;
}

static bool pt1_2(char** f) {
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
