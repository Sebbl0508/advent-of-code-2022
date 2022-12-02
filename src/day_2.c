#include "day_2.h"


static bool pt1_2(char** f);
static uint32_t get_score(char opponent, char myself);
static char get_correct_shape(char opponent, char outcome);

bool day02(void) {
    char* file = read_file_to_string("./inputs/day02.txt");
    char* orig = file;

    pt1_2(&file);

    free(orig);
    return EXIT_SUCCESS;
}


static bool pt1_2(char** f) {
    char* line = strsep(f, "\n");

    char* sep_line = malloc(4 * sizeof(char));

    uint32_t total_score = 0;
    uint32_t correct_total = 0;

    while(line != NULL) {
        // copy pointers to the originals won't get overwritten
        char* mut_sep_line = sep_line;

        // copy the line to a seperate string,
        // so it's easier to work with
        strncpy(sep_line, line, 4);

        char opponent   = strsep(&mut_sep_line, " ")[0];
        char myself     = strsep(&mut_sep_line, " ")[0];

        char correct_myself = get_correct_shape(opponent, myself);

        total_score += get_score(opponent, myself);
        correct_total += get_score(opponent, correct_myself);

        line = strsep(f, "\n");
    }

    printf("[*] (D02-1) Total score: %d\n", total_score);
    printf("[*] (D02-2) Total score: %d\n", correct_total);

    free(sep_line);
    return EXIT_SUCCESS;
}

static char get_correct_shape(char opponent, char outcome) {
    char win_shape = 0;
    char draw_shape = 0;
    char lost_shape = 0;
    switch(opponent) {
        case 'A':
            win_shape = 'Y';
            draw_shape = 'X';
            lost_shape = 'Z';
            break;
        case 'B':
            win_shape = 'Z';
            draw_shape = 'Y';
            lost_shape = 'X';
            break;
        case 'C':
            win_shape = 'X';
            draw_shape = 'Z';
            lost_shape = 'Y';
            break;
        default:
            fprintf(stderr, "[!] Something went wrong (%c)\n", opponent);
            exit(EXIT_FAILURE);
    }

    switch(outcome) {
        case 'X':
            return lost_shape;
        case 'Y':
            return draw_shape;
        case 'Z':
            return win_shape;

        default:
            fprintf(stderr, "[!] Something went wrong (%c)\n", outcome);
    }
}

static uint32_t get_score(char opponent, char myself) {
    uint32_t shape_points = 0;
    switch(myself) {
        // Rock: 1
        case 'X':
            shape_points = 1;
            break;

        // Paper: 2
        case 'Y':
            shape_points = 2;
            break;

        // Scissors: 3
        case 'Z':
            shape_points = 3;
            break;

        default:
            fprintf(stderr, "[!] Something went wrong (%c)\n", myself);
            exit(EXIT_FAILURE);
    }

    char myself_translated = 0;
    switch(myself) {
        case 'X':
            myself_translated = 'A';
            break;
        case 'Y':
            myself_translated = 'B';
            break;
        case 'Z':
            myself_translated = 'C';
            break;
        default:
            fprintf(stderr, "[!] Something went wrong (%c)\n", myself);
            exit(EXIT_FAILURE);
    }

    // 3 points for draw
    if(opponent == myself_translated)
        return shape_points + 3;

    // Rock vs Paper: I WIN!
    if(opponent == 'A' && myself == 'Y')
        return shape_points + 6;

    // Rock vs Scissors: I lost
    if(opponent == 'A' && myself == 'Z')
        return shape_points;

    // Paper vs Rock: I lost
    if(opponent == 'B' && myself == 'X')
        return shape_points;

    // Paper vs Scissors: I WIN!
    if(opponent == 'B' && myself == 'Z')
        return shape_points + 6;

    // Scissors vs Rock: I WIN!
    if(opponent == 'C' && myself == 'X')
        return shape_points + 6;

    // Scissors vs Paper: I lost
    if(opponent == 'C' && myself == 'Y')
        return shape_points;


    // This point should not be reached
    fprintf(stderr, "[!] Reached unreachable point\n");
    exit(EXIT_FAILURE);
}
