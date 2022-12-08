#include "day_8.h"

typedef struct {
    size_t w;
    size_t h;
    // Accessible by [WIDTH * y + x]
    uint8_t* data;
} Grid;

static Grid parse_input(char* f);

static void pt1_2(Grid* grid);
static bool check_visible(Grid* grid, size_t src_x, size_t src_y, int8_t delta_x, int8_t delta_y, size_t* num_visible);

bool day08(void) {
    char* file = read_file_to_string("./inputs/day08.txt");

    Grid grid = parse_input(file);

    pt1_2(&grid);

    free(file);
    free(grid.data);

    printf("\n");
    return EXIT_SUCCESS;
}


static void pt1_2(Grid* grid) {
    // The edge
    size_t visible_trees = (grid->w + grid->h - 2) * 2;

    size_t highest_scenic_score = 0;
    size_t hsc_x = 0, hsc_y = 0;

    for(size_t y = 1; y < grid->h - 1; y++) {
        for(size_t x = 1; x < grid->w - 1; x++) {
            size_t num_visible[4] = {0};

            bool blocked = check_visible(grid, x, y, 1, 0, num_visible);
            blocked     &= check_visible(grid, x, y, 0, 1, num_visible + 1);
            blocked     &= check_visible(grid, x, y, -1, 0, num_visible + 2);
            blocked     &= check_visible(grid, x, y, 0, -1, num_visible + 3);

            size_t scenic_score = num_visible[0]
                                * num_visible[1]
                                * num_visible[2]
                                * num_visible[3];

            if(scenic_score > highest_scenic_score) {
                highest_scenic_score = scenic_score;
                hsc_x = x;
                hsc_y = y;
            }

            if(!blocked)
                visible_trees += 1;
            //printf("%zu %zu (%d) is %s blocked\n", x, y, grid->data[grid->w * y + x], blocked ? "   " : "not");
        }
    }

    printf("[*] (D08-1) Visible trees: %zu\n", visible_trees);
    printf("[*] (D08-2) Highest scenic score: %zu (at %zu %zu)\n", highest_scenic_score, hsc_x, hsc_y);
}

static bool check_visible(Grid* grid, size_t src_x, size_t src_y, int8_t delta_x, int8_t delta_y, size_t* num_visible) {
    uint8_t src = grid->data[grid->w * src_y + src_x];

    // Only need to check for bigger than grid size,
    // since on '0 - 1' the unsigned size_t will underflow :)
    for(size_t x = (src_x + delta_x), y = (src_y + delta_y); (x < grid->w && y < grid->h); x += delta_x, y += delta_y) {
        uint8_t current = grid->data[grid->w * y + x];

        *num_visible += 1;
        if(current >= src)
            return true;
    }

    return false;
}


static Grid parse_input(char* f) {
    Grid grid = {0};

    size_t n_lines = count_lines(f);


    char* line = strsep(&f, "\n");
    size_t line_len = strlen(line);

    grid.data   = calloc(n_lines * line_len, sizeof(uint8_t));
    grid.w      = line_len;
    grid.h      = n_lines;

    size_t idx = 0;
    while(line != NULL) {
        for(size_t i = 0; i < line_len; i++) {
            grid.data[idx] = CHAR_TO_U8(line[i]);
            idx += 1;
        }

        line = strsep(&f, "\n");
    }

    return grid;
}
