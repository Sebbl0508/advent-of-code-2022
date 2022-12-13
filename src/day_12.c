#include "day_12.h"

typedef struct {
    uint64_t vertex_num;
    uint64_t** edges;
} DijGraph;

typedef struct {
    uint64_t x;
    uint64_t y;
} Vec2_u64;

typedef struct {
    int64_t x;
    int64_t y;
} Vec2_i64;

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
static Vec2_i64* get_possible_directions(Day12_Data* data, size_t* po_num_directions);
static uint8_t get_height(Day12_Data* data, uint64_t x, uint64_t y);
static bool can_step(Day12_Data* data, uint64_t xpoint, uint64_t ypoint, uint64_t xother, uint64_t yother);

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
    uint64_t* dist_arr = calloc(data->width * data->height, sizeof(uint64_t));


    for(size_t i = 0; i < data->width * data->height; i++) {
        dist_arr[i] = UINT64_MAX;
    }


    // While current point is not the end point, continue
    while(!vec2_cmp(&data->cur_point, &data->end_point)) {
        size_t num_possible_dirs = 0;
        Vec2_i64* possible_dirs = get_possible_directions(data, &num_possible_dirs);
    }

    printf("JOA\n");

    free(dist_arr);
}

static Vec2_i64* get_possible_directions(Day12_Data* data, size_t* po_num_directions) {
    static Vec2_i64 *direction_cache = NULL;
    if (direction_cache == NULL) {
        direction_cache = calloc(4, sizeof(Vec2_i64));
    }

    size_t next_cache_idx = 0;
    Vec2_u64 cur_point = data->cur_point;

    if (cur_point.x != 0 && can_step(data, cur_point.x-1, cur_point.y, cur_point.x, cur_point.y)) {
        direction_cache[next_cache_idx] = (Vec2_i64) {-1, 0};
        next_cache_idx += 1;
    }
    if (cur_point.y != 0 && can_step(data, cur_point.x, cur_point.y-1, cur_point.x, cur_point.y)) {
        direction_cache[next_cache_idx] = (Vec2_i64) {0, -1};
        next_cache_idx += 1;
    }
    if (cur_point.x < data->width && can_step(data, cur_point.x+1, cur_point.y, cur_point.x, cur_point.y)) {
        direction_cache[next_cache_idx] = (Vec2_i64) {1, 0};
        next_cache_idx += 1;
    }
    if (cur_point.y < data->height && can_step(data, cur_point.x, cur_point.y+1, cur_point.x, cur_point.y)) {
        direction_cache[next_cache_idx] = (Vec2_i64) {0, 1};
        next_cache_idx += 1;
    }

    *po_num_directions = next_cache_idx;

    return direction_cache;
}

static bool can_step(Day12_Data* data, uint64_t xpoint, uint64_t ypoint, uint64_t xother, uint64_t yother) {
    return (get_height(data, xother, yother) <= (get_height(data, xpoint, ypoint) + 1));
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
