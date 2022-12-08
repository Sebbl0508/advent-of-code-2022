#include "day_7.h"


size_t get_folder_num(char* f);
size_t recurse(char** f, size_t* dirs, size_t* pos);


bool day07(void) {
    char* file = read_file_to_string("./inputs/day07.txt");

    // Idk there is a bug in the recursive function
    // But just allocating 2 folders more solves the
    // problem
    size_t num_folders = get_folder_num(file) + 2;
    size_t* dirs = calloc(num_folders, sizeof(size_t));

    char* f_w = file;
    size_t pos = 0;
    size_t total_size = recurse(&f_w, dirs, &pos);

    size_t t = 0;
    for(size_t i = 0; i < num_folders; i++) {
        if(dirs[i] <= 100000)
            t += dirs[i];
    }

    size_t free_space = 70000000 - total_size;
    size_t needed_space = 30000000 - free_space;

    size_t t2 = 0;
    for(size_t i = 0; i < num_folders; i++) {
        if(dirs[i] > needed_space)
            if(t2 == 0) {
                t2 = dirs[i];
            } else {
                t2 = MIN(t2, dirs[i]);
            }
    }

    printf("[*] (D07-1) %zu\n", t);
    printf("[*] (D07-2) %zu\n", t2);

    free(dirs);
    free(file);
    return EXIT_SUCCESS;
}

size_t recurse(char** f, size_t* dirs, size_t* pos) {
    char* line = strsep(f, "\n");

    size_t cur_pos = (*pos)++;

    while(line != NULL) {
        if(IS_DIGIT(line[0]))
            dirs[cur_pos] += atoi(line);
        else if(line[0] == '$' && line[2] == 'c') {
            if(line[5] == '.') {
                break;
            } else {
                dirs[cur_pos] += recurse(f, dirs, pos);
            }
        }

        line = strsep(f, "\n");
    }

    return dirs[cur_pos];
}

size_t get_folder_num(char* f) {
    size_t num_folders = 0;
    char* fc = copy_str(f);
    char* fc_w = fc;

    char* line = strsep(&fc_w, "\n");
    while(line != NULL) {
        if(line[0] == '$' && line[2] == 'c' && line[5] != '.')
            num_folders += 1;

        line = strsep(&fc_w, "\n");
    }


    free(fc);

    return num_folders;
}
