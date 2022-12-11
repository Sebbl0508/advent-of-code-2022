#include "day_9.h"

#include <math.h>

typedef enum {
    MoveDir_Up,
    MoveDir_Down,
    MoveDir_Left,
    MoveDir_Right,
} MoveDir;

typedef struct {
    int64_t x;
    int64_t y;
} Vec2;

typedef struct {
    Vec2 head;
    Vec2 tail;
} Rope;

static void pt1(char* f);
static MoveDir get_move_dir(char dir);
static void move_rope(Rope* rope, MoveDir dir, size_t num_steps, Vec2** visited_arr, size_t* visited_arr_len);
static void move_point(Vec2* v, MoveDir dir);
static int64_t normalize_i64(int64_t i);

static bool vec2_cmp(Vec2 a, Vec2 b);

static bool arr_vec2_contains(Vec2* arr, size_t n, Vec2 v);
static Vec2* arr_vec2_push(Vec2* arr, size_t* n, Vec2 v);

bool day09(void) {
    char* file = read_file_to_string("./inputs/day09.txt");

    pt1(file);

    printf("\n");
    return EXIT_SUCCESS;
}

static void pt1(char* f) {
    char* line = strsep(&f, "\n");

    // Both head & tail start at (0, 0)
    Rope rope = {0};
    Vec2* visited = malloc(sizeof(Vec2));
    size_t visited_len = 1;

    // The tail also visits the starting point
    visited[0] = (Vec2){ 0, 0 };

    while(line != NULL) {
        if(!IS_DIGIT(line[2])) {
            fprintf(stderr, "[!] The 3rd character should've been a digit: '%s'\n", line);
            exit(EXIT_FAILURE);
        }

        MoveDir dir = get_move_dir(line[0]);
        size_t n_steps = strtol(&line[2], NULL, 10);

        move_rope(&rope, dir, n_steps, &visited, &visited_len);

        line = strsep(&f, "\n");
    }

    printf("[*] (D09-1) Head is at (%ld, %ld), tail at (%ld, %ld). Tail visited %ld spots\n", rope.head.x, rope.head.y, rope.tail.x, rope.tail.y, visited_len);
}

static void move_rope(Rope* rope, MoveDir dir, size_t num_steps, Vec2** visited_arr, size_t* visited_arr_len) {
    for(size_t i = 0; i < num_steps; i++) {
        // Move head first
        move_point(&rope->head, dir);


        // TODO: The following code for all knots
        Vec2 delta = {
            .x = rope->head.x - rope->tail.x,
            .y = rope->head.y - rope->tail.y
        };

        if(llabs(delta.x) > 1 || llabs(delta.y) > 1) {
            rope->tail.x += normalize_i64(delta.x);
            rope->tail.y += normalize_i64(delta.y);

            if(!arr_vec2_contains(*visited_arr, *visited_arr_len, rope->tail)) {
                *visited_arr = arr_vec2_push(*visited_arr, visited_arr_len, rope->tail);
            }
        }
    }
}

static int64_t normalize_i64(int64_t i) {
    if(i > 0) {
        return 1;
    } else if(i < 0) {
        return -1;
    } else {
        return 0;
    }
}

static MoveDir get_move_dir(char dir) {
    switch(dir) {
        case 'R':
            return MoveDir_Right;
        case 'L':
            return MoveDir_Left;
        case 'U':
            return MoveDir_Up;
        case 'D':
            return MoveDir_Down;
        default:
            fprintf(stderr, "[!] Not a valid direction (%c)\n", dir);
            exit(EXIT_FAILURE);
    }
}

static void move_point(Vec2* v, MoveDir dir) {
    switch(dir) {
        case MoveDir_Left:
            v->x -= 1;
            break;
        case MoveDir_Right:
            v->x += 1;
            break;
        case MoveDir_Up:
            v->y += 1;
            break;
        case MoveDir_Down:
            v->y -= 1;
            break;
    }
}

static bool arr_vec2_contains(Vec2* arr, size_t n, Vec2 v) {
    for(size_t i = 0; i < n; i++) {
        if(vec2_cmp(arr[i], v))
            return true;
    }

    return false;
}

static Vec2* arr_vec2_push(Vec2* arr, size_t* n, Vec2 v) {
    Vec2* a = realloc(arr, ((*n)+1) * sizeof(Vec2));
    a[*n] = v;

    *n += 1;

    return a;
}

static bool vec2_cmp(Vec2 a, Vec2 b) {
    return a.x == b.x && a.y == b.y;
}
