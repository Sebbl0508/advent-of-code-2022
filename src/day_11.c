#include "day_11.h"


typedef enum {
    OpVal_Old,
    OpVal_Num,
} OperationVal;

typedef enum {
    OpSign_Plus,
    OpSign_Mult,
    OpSign_Minus,
    OpSign_Div,
} OpSign;

typedef struct {
    OpSign sign;
    bool old_val;
    uint64_t num;
} Operation;

typedef struct {
    size_t num_items;
    uint64_t* items;
    size_t divisible_by;
    size_t true_idx;
    size_t false_idx;
    size_t inspection_count;
    Operation op;
} Monkey;

static void pt1(char* f);

// Takes the first 'Monkey X' definition line and returns the pointer at the end
static Monkey parse_monkey(char** f);
static void destroy_monkey(Monkey* m);
static uint64_t* parse_starting_items(char* s, size_t* len);
static Operation parse_op(char const* op_begin);
static void inspect_item(Monkey* m);
static void throw_item(Monkey* from, Monkey* to);
static size_t get_destination_monkey(Monkey* m);
static void print_monkeys(Monkey* monkeys, size_t n);

bool day11(void) {
    char* file = read_file_to_string("./inputs/day11.txt");


    pt1(file);

    free(file);
    printf("\n");
    return EXIT_SUCCESS;
}


// TODO: Make a 'parse_monkeys' function that returns 'Monkey*' -> memcpy for part2
static void pt1(char* f) {
    char* line = strsep(&f, "\n");

    Monkey* monkeys = NULL;
    size_t monkey_num = 0;

    // Parse all monkeys first
    while(line != NULL) {
        // New monkey
        if(line[0] == 'M') {
            monkeys = realloc(monkeys, (monkey_num+1) * sizeof(Monkey));
            monkeys[monkey_num] = parse_monkey(&f);
            monkey_num += 1;
        }

        line = strsep(&f, "\n");
    }

    uint64_t prod = 1;
    for(uint64_t i = 0; i < monkey_num; i++) {
        prod *= monkeys[i].divisible_by;
    }

    // Adjust for part1/2
    const size_t rounds = 10000;

    // Do 20 rounds
    for(size_t round = 0; round < rounds; round++) {
        for(size_t i = 0; i < monkey_num; i++) {
            if(monkeys[i].num_items == 0)
                continue;

            // All items
            const size_t num_monkey_items = monkeys[i].num_items;
            for(size_t j = 0; j < num_monkey_items; j++) {

                // TODO: Debug inspecting & math
                inspect_item(&monkeys[i]);
                monkeys[i].items[0] = monkeys[i].items[0] % prod;
                //monkeys[i].items[0] /= 3;

                size_t dest_monkey = get_destination_monkey(&monkeys[i]);
                throw_item(&monkeys[i], &monkeys[dest_monkey]);
            }
        }
        //printf("[*] After round %zu\n", round + 1);
        //print_monkeys(monkeys, monkey_num);
    }

    size_t* inspection_counts = calloc(monkey_num, sizeof(uint64_t));
    for(size_t i = 0; i < monkey_num; i++) {
        inspection_counts[i] = monkeys[i].inspection_count;
    }
    qsort(inspection_counts, monkey_num, sizeof(size_t), compare_sizet);

    printf("[*] (D11-1/2) %ld\n", inspection_counts[monkey_num-1] * inspection_counts[monkey_num-2]);

    free(inspection_counts);
    inspection_counts = NULL;

    for(size_t i = 0; i < monkey_num; i++) {
        destroy_monkey(&monkeys[i]);
    }
    // Release the monkeys
    free(monkeys);
}

static void print_monkeys(Monkey* monkeys, size_t n) {
    for(size_t i = 0; i < n; i++) {
        printf("Monkey %zu: ", i);
        for(size_t j = 0; j < monkeys[i].num_items; j++) {
            printf("%ld, ", monkeys[i].items[j]);
        }
        printf("\n");
    }
    printf("\n");
}

static void inspect_item(Monkey* m) {
    uint64_t right_value;

    // if it's old [op] old
    if(m->op.old_val) {
        right_value = m->items[0];
    } else {
        right_value = m->op.num;
    }

    switch(m->op.sign) {
        case OpSign_Mult:
            m->items[0] *= right_value;
            break;
        case OpSign_Plus:
            m->items[0] += right_value;
            break;
        case OpSign_Div:
            m->items[0] /= right_value;
            break;
        case OpSign_Minus:
            m->items[0] -= right_value;
            break;
    }

    m->inspection_count += 1;
}

static size_t get_destination_monkey(Monkey* m) {
    if(m->items[0] % m->divisible_by == 0) {
        return m->true_idx;
    } else {
        return m->false_idx;
    }
}

// This is very inefficient because it (re)allocs 3 arrays every time an item is thrown
static void throw_item(Monkey* from, Monkey* to) {
    uint64_t v = from->items[0];

    // Allocates a new array,
    // moves all but the first element of 'from' into the 'buf' array
    // then frees the monkey's item array and assigns it the 'buf' pointer
    if(from->items == NULL || from->num_items == 0) {
        fprintf(stderr, "[!] This is not supposed to happend\n");
        exit(EXIT_FAILURE);
    }

    // If there is only 1 item left, don't copy it to a buffer
    // since after throwing it, the monkey has no items left
    //
    // (If the monkey has no items, this function should NOT be called at all)

    if(from->num_items <= 1) {
        free(from->items);
        from->items = NULL;
        from->num_items -= 1;
    } else {
        uint64_t* buf = calloc(from->num_items-1, sizeof(uint64_t));
        memmove(buf, &from->items[1], (from->num_items-1) * sizeof(uint64_t));

        free(from->items);
        from->items = buf;
        buf = NULL;

        from->num_items -= 1;
    }

    // Increase the size of the destination array and push the new item into it
    to->items = realloc(to->items, (to->num_items + 1) * sizeof(uint64_t));
    to->items[to->num_items] = v;
    to->num_items += 1;
}

static Monkey parse_monkey(char** f) {
    char *line = strsep(f, "\n");
    Monkey monkey = {0};

    for (size_t i = 0; i < 5; i++) {
        if (line[2] == 'S') {
            // Starting items
            char* begin_items = &line[18];
            monkey.items = parse_starting_items(begin_items, &monkey.num_items);
        } else if(line[2] == 'O') {
            // Operation
            monkey.op = parse_op(&line[23]);
        } else if(line[2] == 'T') {
            // Test
            monkey.divisible_by = strtoll(&line[21], NULL, 10);
        } else if(line[4] == 'I') {
            // If true/false
            if(line[7] == 't') {
                monkey.true_idx = strtoll(&line[29], NULL, 10);
            } else {
                monkey.false_idx = strtoll(&line[30], NULL, 10);
            }
        }

        line = strsep(f, "\n");
    }

    return monkey;
}

static Operation parse_op(char const* op_begin) {
    Operation op = {0};
    switch (op_begin[0]) {
        case '*':
            op.sign = OpSign_Mult;
            break;
        case '+':
            op.sign = OpSign_Plus;
            break;
        case '-':
            op.sign = OpSign_Minus;
            break;
        case '/':
            op.sign = OpSign_Div;
            break;
        default:
            fprintf(stderr, "[!] Invalid operation sign (%c)\n", op_begin[0]);
            exit(EXIT_FAILURE);
    }

    char const* val_begin = op_begin + 2;
    if(strncmp(val_begin, "old", 3) == 0) {
        op.old_val = true;
    } else {
        op.num = strtoll(val_begin, NULL, 10);
    }

    return op;
}

static uint64_t* parse_starting_items(char* s, size_t* len) {
    char* line = strdup(s);
    char* line_o = line;

    char* num = strsep(&line, ",");

    uint64_t* x = NULL;

    size_t i = 0;
    for(i = 0; num != NULL; i++) {
        uint64_t n = strtol(num, NULL, 10);

        x = realloc(x, (i+1) * sizeof(uint64_t));
        x[i] = n;

        num = strsep(&line, ",");
    }

    *len = i;
    free(line_o);

    return x;
}

static void destroy_monkey(Monkey* m) {
    free(m->items);
    memset(m, 0, sizeof(Monkey));
    // Monkey is dead
}
