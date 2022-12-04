#include "day_3.h"

static bool pt1(char** f);
static uint32_t get_prio(char item);
static bool string_contains_char(char* string, size_t len, char c);

bool day03(void) {
    char* file = read_file_to_string("./inputs/day03.txt");
    char* orig = file;

    pt1(&file);

    free(orig);
    return EXIT_SUCCESS;
}


static bool pt1(char** f) {
    char* line = strsep(f, "\n");


    uint32_t total = 0;

    while(line != NULL) {
        size_t line_len = strlen(line);
        size_t half_line_len = line_len / 2;

        char* first_half = calloc(half_line_len, sizeof(char));

        for(size_t i = 1; i <= line_len; i++) {
            if(i <= half_line_len) {
                first_half[i-1] = line[i-1];
            } else {
                if(string_contains_char(first_half, half_line_len, line[i-1])) {
                    total += get_prio(line[i-1]);

                    // GOTO BAD but ¯\_(ツ)_/¯
                    goto double_found;
                }
            }
        }
        
double_found:
        free(first_half);
        line = strsep(f, "\n");
    }

    printf("[*] (D03-1) Sum of priorities: %d\n", total);
}

static uint32_t get_prio(char item) {
    if(item >= 'a' && item <= 'z') {
        return item - ('a' - 1);
    } else if(item >= 'A' && item <= 'Z') {
        return item - ('A' - 27);
    } else {
        fprintf(stderr, "[!] Reached the unreachable, or something fucked up\n");
        exit(EXIT_FAILURE);
    }
}

static bool string_contains_char(char* string, size_t len, char c) {
    for(size_t i = 0; i < len; i++) {
        if(string[i] == c)
            return true;
    }

    return false;
}
