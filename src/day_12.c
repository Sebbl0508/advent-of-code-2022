#include "day_12.h"

#include "utils/smpl_queue.h"


typedef struct {
    uint64_t x;
    uint64_t y;
} Vec2_u64;

typedef struct {
    int64_t x;
    int64_t y;
} Vec2_i64;

typedef struct {
    Vec2_u64 pos;
    int64_t dist;
} BfsNode;

typedef struct {
    size_t height;
    size_t width;
    Vec2_u64 starting_point;
    Vec2_u64 end_point;
    Vec2_u64 cur_point;
    // Accessible via [WIDTH * y + x]
    uint8_t* heightmap;
} Day12_Data;

static bool vec2_cmp(Vec2_u64* a, Vec2_u64* b);
static Day12_Data parse_input(char* f);
static Day12_Data clone_data(Day12_Data* v);
static void destroy_data(Day12_Data* v);
static Vec2_i64* get_possible_directions(Day12_Data* data, size_t* po_num_directions, bool* explored_grid);
static uint8_t get_height(Day12_Data* data, uint64_t x, uint64_t y);
static bool can_step(Day12_Data* data, uint64_t xpoint, uint64_t ypoint, uint64_t xother, uint64_t yother, const bool* explored_grid);
static void bfs(Day12_Data* data);

static void pt1(Day12_Data* data);

bool day12(void) {
    char* file = read_file_to_string("./inputs/day12ex.txt");

    Day12_Data data = parse_input(file);
    pt1(&data);

    free(file);
    printf("\n");
    return EXIT_SUCCESS;
}

static void pt1(Day12_Data* data) {
    bfs(data);

    printf("JOA\n");
}

static void bfs(Day12_Data* data) {
    // What places in the grid were explored already
    bool* explored_grid = calloc(data->width * data->height, sizeof(bool));


    SQueue q = squeue_create(sizeof(BfsNode));
    BfsNode root = (BfsNode) { data->starting_point, 0 };
    squeue_push(&q, &root);
    explored_grid[data->width * data->starting_point.y + data->starting_point.x] = true;

    while(!squeue_is_empty(&q)) {
        // I definitely won't regret setting this like that
        if(q.head == NULL || q.tail == NULL) {
            q.head = NULL;
        }
        BfsNode cur_node = *(BfsNode *) squeue_pop_front(&q);
        data->cur_point = cur_node.pos;

        if(vec2_cmp(&data->cur_point, &data->end_point)) {
            // TODO: Goal reached
            printf("Dist %ld\n", cur_node.dist);
        }

        /*
        if(explored_grid[data->width * data->cur_point.y + data->cur_point.x]) {
            // TODO: When the point was explored already
        }
        */

        // Get all possible directions & iterate over them
        size_t num_possible_direction = 0;
        Vec2_i64* possible_directions = get_possible_directions(data, &num_possible_direction, explored_grid);
        for(size_t i = 0; i < num_possible_direction; i++) {
            Vec2_i64 dir = possible_directions[i];

            Vec2_u64 new_pos = (Vec2_u64){ data->cur_point.x + dir.x, data->cur_point.y + dir.y };
            BfsNode new_node = (BfsNode){ new_pos, cur_node.dist + 1 };
            squeue_push(&q, &new_node);
            explored_grid[data->width * new_pos.y + new_pos.x] = true;
        }
    }

    /*
    // While current point is not the end point, continue
    while(!vec2_cmp(&data->cur_point, &data->end_point)) {
        size_t num_possible_dirs = 0;
        Vec2_i64* possible_dirs = get_possible_directions(data, &num_possible_dirs);
    }
    */

    free(explored_grid);
    squeue_destroy(&q);
}

static Vec2_i64* get_possible_directions(Day12_Data* data, size_t* po_num_directions, bool* explored_grid) {
    static Vec2_i64 *direction_cache = NULL;
    if (direction_cache == NULL) {
        direction_cache = calloc(4, sizeof(Vec2_i64));
    }

    size_t next_cache_idx = 0;
    Vec2_u64 cur_point = data->cur_point;

    if (cur_point.x != 0 && can_step(data, cur_point.x, cur_point.y, cur_point.x-1, cur_point.y, explored_grid)) {
        direction_cache[next_cache_idx] = (Vec2_i64) {-1, 0};
        next_cache_idx += 1;
    }
    if (cur_point.y != 0 && can_step(data, cur_point.x, cur_point.y, cur_point.x, cur_point.y-1, explored_grid)) {
        direction_cache[next_cache_idx] = (Vec2_i64) {0, -1};
        next_cache_idx += 1;
    }
    if (cur_point.x < data->width && can_step(data, cur_point.x, cur_point.y, cur_point.x+1, cur_point.y, explored_grid)) {
        direction_cache[next_cache_idx] = (Vec2_i64) {1, 0};
        next_cache_idx += 1;
    }
    if (cur_point.y < data->height && can_step(data, cur_point.x, cur_point.y, cur_point.x, cur_point.y+1, explored_grid)) {
        direction_cache[next_cache_idx] = (Vec2_i64) {0, 1};
        next_cache_idx += 1;
    }

    *po_num_directions = next_cache_idx;

    return direction_cache;
}

static bool can_step(Day12_Data* data, uint64_t xpoint, uint64_t ypoint, uint64_t xother, uint64_t yother, const bool* explored_grid) {
    return (get_height(data, xother, yother) <= (get_height(data, xpoint, ypoint) + 1) && !(explored_grid[data->width * yother + xother]));
}

static uint8_t get_height(Day12_Data* data, uint64_t x, uint64_t y) {
    return data->heightmap[data->width * y + x];
}

static Day12_Data parse_input(char* f) {
    char* line = strsep(&f, "\n");

    Day12_Data data = {0};
    data.width = strlen(line);

    while(line != NULL) {
        data.heightmap = realloc(data.heightmap, ((data.height+1) * data.width) * sizeof(uint8_t));

        for(size_t i = 0; i < data.width; i++) {
            if(line[i] == 'S') {
                data.starting_point = (Vec2_u64) {
                    .x = i,
                    .y = data.height
                };
                data.heightmap[data.width * data.height + i] = 0;
            } else if(line[i] == 'E') {
                data.end_point = (Vec2_u64) {
                    .x = i,
                    .y = data.height
                };
                data.heightmap[data.width * data.height + i] = 'z' - 97;
            } else {
                data.heightmap[data.width * data.height + i] = line[i] - 97;
            }
        }

        data.height += 1;
        line = strsep(&f, "\n");
    }
    data.cur_point = data.starting_point;

    return data;
}

static bool vec2_cmp(Vec2_u64* a, Vec2_u64* b) {
    return (a->x == b->x && a->y == b->y);
}

static Day12_Data clone_data(Day12_Data* v) {
    Day12_Data clone = *v;
    clone.heightmap = NULL;
    clone.heightmap = calloc(clone.height * clone.width, sizeof(uint8_t));
    memcpy(clone.heightmap, v->heightmap, (clone.height * clone.width) * sizeof(uint8_t));

    return clone;
}

static void destroy_data(Day12_Data* v) {
    free(v->heightmap);
    memset(v, 0, sizeof(Day12_Data));
}
