#include "day_4.h"

typedef struct {
    int from;
    int to;
} Range;


// why tf did i use bool before?!?
static void pt1(char** f);

// Return the number of overlapping numbers
static uint32_t num_overlapping(Range first, Range second);

// Checks if any of the both ranges fully contain the other
static bool does_fully_contain(Range first, Range second);


bool day04(void) {
    char* file = read_file_to_string("./inputs/day04.txt");
    char* file_orig = file;

    pt1(&file);

    free(file_orig);

    printf("\n");
    return EXIT_SUCCESS;
}


static void pt1(char** f) {
    char* line = strsep(f, "\n");

    uint32_t total_fully_contain = 0;
    uint32_t total_overlapping = 0;

    char* line_copy = NULL;
    while(line != NULL) {
        size_t line_len = strlen(line);

        // Length of the string + '\0'
        line_copy = realloc(line_copy, line_len + 1);
        memcpy(line_copy, line, line_len + 1);


        // Copy the original ptr, so it can be
        // safely manipulated by 'strsep'
        char* line_copy_w = line_copy;
        char* elf1 = strsep(&line_copy_w, ",");
        char* elf2 = strsep(&line_copy_w, ",");

        Range r_elf1 = {0};
        Range r_elf2 = {0};

        r_elf1.from = atoi(strsep(&elf1, "-"));
        r_elf1.to   = atoi(strsep(&elf1, "-"));

        r_elf2.from = atoi(strsep(&elf2, "-"));
        r_elf2.to   = atoi(strsep(&elf2, "-"));

        if(num_overlapping(r_elf1, r_elf2) > 0)
            total_overlapping += 1;

        if(does_fully_contain(r_elf1, r_elf2))
            total_fully_contain += 1;

        line = strsep(f, "\n");
    }
    free(line_copy);

    printf("[*] (D04-1) Sum of fully overlapping pairs: %d\n", total_fully_contain);
    printf("[*] (D04-2) Sum of overlapping pairs: %d\n", total_overlapping);
}

static bool does_fully_contain(Range first, Range second) {
    // Find out which of the ranges is smaller
    int first_diff = abs(first.to+1 - first.from);
    int second_diff = abs(second.to+1 - second.from);
    int smaller_diff = MIN(first_diff, second_diff);

    uint32_t overlapping = num_overlapping(first, second);

    // if the number of overlapping numbers is
    // the whole smaller range, the bigger range
    // fully contains it
    return smaller_diff == (int)overlapping;
}

static uint32_t num_overlapping(Range first, Range second) {
    uint32_t overlapping = 0;

    for(int i = first.from; i <= first.to; i++) {
        for(int j = second.from; j <= second.to; j++) {
            if(i == j)
                overlapping += 1;
        }
    }

    return overlapping;
}
