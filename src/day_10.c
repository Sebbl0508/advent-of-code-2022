#include "day_10.h"

typedef enum {
    Inst_Addx,
    Inst_Noop,
} Instruction;

typedef struct {
    size_t cycle_counter;
    size_t total_strength;
    int64_t reg_x;

    // accessible via [WIDTH * y + x]
    bool* screen;
} CPU;

static void pt1(char* f);
static Instruction get_instruction(char* line);
static CPU cpu_init(void);
static void cpu_destroy(CPU* cpu);
static void cpu_tick(CPU* cpu);
static void cpu_draw_sprite(CPU* cpu);
static void cpu_print_screen(CPU* cpu);
static void cpu_execute_instruction(CPU* cpu, Instruction instruction, char* line);


bool day10(void) {
    char* file = read_file_to_string("./inputs/day10.txt");

    pt1(file);

    free(file);
    printf("\n");
    return EXIT_SUCCESS;
}



static void pt1(char* f) {
    char* line = strsep(&f, "\n");

    CPU cpu = cpu_init();

    Instruction inst;

    while(line != NULL) {
        inst = get_instruction(line);
        cpu_execute_instruction(&cpu, inst, line);

        line = strsep(&f, "\n");
    }
    printf("[*] (D10-1) Total strength: %zu\n", cpu.total_strength);
    printf("[*] (D10-2) Screen: \n");
    cpu_print_screen(&cpu);


    cpu_destroy(&cpu);
}


static CPU cpu_init(void) {
    CPU cpu = {
        .cycle_counter = 0,
        .reg_x = 1,
        .total_strength = 0,
        .screen = calloc(6 * 40, sizeof(bool))
    };
    return cpu;
}

static void cpu_print_screen(CPU* cpu) {
    char* line_buffer = calloc(41, sizeof(char));

    for(size_t y = 0; y < 6; y++) {
        for(size_t x = 0; x < 40; x++) {
            line_buffer[x] = cpu->screen[40 * y + x] ? '#' : '.';
        }
        printf("%s\n", line_buffer);
    }

    free(line_buffer);
}

static void cpu_destroy(CPU* cpu) {
    free(cpu->screen);
}

static void cpu_execute_instruction(CPU* cpu, Instruction instruction, char* line) {
    if(instruction == Inst_Noop) {
        cpu_tick(cpu);
    } else {
        cpu_tick(cpu);
        cpu_tick(cpu);
        cpu->reg_x += strtol(&line[5], NULL, 10);
    }
}

static void cpu_tick(CPU* cpu) {
    cpu->cycle_counter += 1;

    if(cpu->cycle_counter % 40 == 20) {
        cpu->total_strength += cpu->cycle_counter * cpu->reg_x;
    }
    cpu_draw_sprite(cpu);
}

static void cpu_draw_sprite(CPU* cpu) {
    size_t line = (cpu->cycle_counter) / 40;
    size_t row = (cpu->cycle_counter-1) % 40;

    int64_t diff = llabs((int64_t)row - cpu->reg_x);

    if(diff <= 1)
        cpu->screen[40 * line + row] = true;
}

static Instruction get_instruction(char* line) {
    const int inst_str_len = 4;

    if(strncmp(line, "noop", inst_str_len) == 0) {
        return Inst_Noop;
    } else if(strncmp(line, "addx", inst_str_len) == 0) {
        return Inst_Addx;
    }

    fprintf(stderr, "[!] Invalid instruction line: '%s'\n", line);
    exit(EXIT_FAILURE);
}
