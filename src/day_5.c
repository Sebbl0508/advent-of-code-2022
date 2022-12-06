#include "day_5.h"

typedef struct {
    size_t num_crates;
    // The first ones are the bottom ones
    char* crates;
} CargoStack;

void stack_move_mul(CargoStack* from, CargoStack* to, size_t n);
void stack_push_char(CargoStack* stack, char c);
void stack_reverse(CargoStack* stack);
char stack_pop_char(CargoStack* stack);
void stack_free(CargoStack* stack);

void pt1_2(char** f, uint8_t pt);
size_t get_num_stacks(char** f);

bool day05(void) {
    char* file = read_file_to_string("./inputs/day05.txt");

    for(size_t i = 0; i < 2; i++) {
        char* pt1_copy = copy_str(file);
        char* pt1_orig = pt1_copy;
        pt1_2(&pt1_copy, i);
        free(pt1_orig);
    }

    free(file);

    printf("\n");
    return EXIT_SUCCESS;
}


void pt1_2(char** f, uint8_t pt) {
    char* line_copy = copy_str(*f);
    char* line = strsep(f, "\n");


    char* line_copy_w = line_copy;
    size_t num_stacks = get_num_stacks(&line_copy_w);
    free(line_copy);


    CargoStack* stacks = calloc(num_stacks, sizeof(CargoStack));

    // Iterate over the stack definition
    while(line != NULL && line[1] != '1') {
        size_t cur_stack_idx = 0;
        for(size_t i = 1; i < strlen(line); i += 4) {
            if(line[i] != ' ') {
                char crate_c = line[i];
                stack_push_char(&stacks[cur_stack_idx], crate_c);
            }
            cur_stack_idx += 1;
        }

        line = strsep(f, "\n");
    }

    // Reverse all stacks
    for(size_t i = 0; i < num_stacks; i++) {
        stack_reverse(&stacks[i]);
    }

    while(line != NULL) {
        // move instruction
        if(line[0] == 'm') {
            uint32_t amt, from, to;
            sscanf(line, "move %u from %u to %u", &amt, &from, &to);

            if(pt == 0) {
                for(size_t i = 0; i < amt; i++) {
                    char tmp = stack_pop_char(&stacks[from-1]);
                    stack_push_char(&stacks[to-1], tmp);
                }
            } else if(pt == 1) {
                stack_move_mul(&stacks[from-1], &stacks[to-1], amt);
            } else {
                fprintf(stderr, "[!] This shouldn't happen\n");
                exit(EXIT_FAILURE);
            }
        }

        line = strsep(f, "\n");
    }

    // Print top crates & free all the stacks
    printf("[*] (D05-%d) Top crates: ", pt + 1);
    for(size_t i = 0; i < num_stacks; i++) {
        printf("%c", stack_pop_char(&stacks[i]));
        stack_free(&stacks[i]);
    }
    printf("\n");


    free(stacks);
}


size_t get_num_stacks(char** f) {
    char* line = strsep(f, "\n");

    long highest_num = 0;

    while(line != NULL) {
        if(line[0] == ' ' && line[1] == '1') {
            char* line_cpy = strsep(&line, " ");

            while(line_cpy != NULL) {
                if(line_cpy[0] != '\0') {
                    highest_num = MAX(highest_num, strtol(line_cpy, NULL, 10));
                }

                line_cpy = strsep(&line, " ");
            }
            break;
        }

        line = strsep(f, "\n");
    }
    if(highest_num < 0) {
        fprintf(stderr, "[!] Somehow, the size of the stack is negative (%ld)\n", highest_num);
        exit(EXIT_FAILURE);
    }

    return (size_t)highest_num;
}

void stack_push_char(CargoStack* stack, char c) {
    stack->crates = realloc(stack->crates, (stack->num_crates + 1) * sizeof(char));
    stack->crates[stack->num_crates] = c;

    stack->num_crates += 1;
}

char stack_pop_char(CargoStack* stack) {
    char ret = stack->crates[stack->num_crates-1];
    stack->crates = realloc(stack->crates, (stack->num_crates-1) * sizeof(char));

    stack->num_crates -= 1;

    return ret;
}

void stack_reverse(CargoStack* stack) {
    size_t start = 0;
    size_t end = stack->num_crates - 1;

    while(start < end) {
        char tmp = stack->crates[start];
        stack->crates[start] = stack->crates[end];
        stack->crates[end]   = tmp;

        start += 1;
        end   -= 1;
    }
}

void stack_move_mul(CargoStack* from, CargoStack* to, size_t n) {
    char* tmp = calloc(n, sizeof(char));
    if(n > from->num_crates) {
        fprintf(stderr, "[!] Tried taking more crates from a stack than it contains (%dx)\n", n);
        exit(EXIT_FAILURE);
    }

    char* from_start_ptr = from->crates + from->num_crates - (n * sizeof(char));

    // Need to realloc first, because the array may move
    to->crates = realloc(to->crates, to->num_crates + n);
    char* to_start_ptr = to->crates + to->num_crates;

    memcpy(to_start_ptr, from_start_ptr, n * sizeof(char));

    // Shrink src stack
    from->crates = realloc(from->crates, from->num_crates - n);

    to->num_crates += n;
    from->num_crates -= n;

    free(tmp);
}

void stack_free(CargoStack* stack) {
    free(stack->crates);
    memset(stack, 0, sizeof(CargoStack));
}
