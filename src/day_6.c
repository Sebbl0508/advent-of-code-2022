#include "day_6.h"

static size_t characters_until_unique(char* f, size_t marker_len);
static bool str_contains_x_unique(const char* s, size_t n);

bool day06(void) {
    char* file = read_file_to_string("./inputs/day06.txt");

    size_t start_of_packet_marker = characters_until_unique(file, 4);
    printf("[*] (D06-1) Processed %zu characters before start of packet\n", start_of_packet_marker + 4);

    size_t start_of_msg_marker = characters_until_unique(file, 14);
    printf("[*] (D06-2) Processed %zu characters before start of message\n", start_of_msg_marker + 14);

    free(file);
    printf("\n");
    return EXIT_SUCCESS;
}

static size_t characters_until_unique(char* f, size_t marker_len) {
    for(size_t i = 0; i < strlen(f); i++) {
        if(str_contains_x_unique(&f[i], marker_len)) {
            return i;
        }
    }

    return 0;
}

static bool str_contains_x_unique(const char* s, size_t n) {
    for(size_t i = 0; i < n; i++) {
        for(size_t j = i + 1; j < n; j++) {
            if(s[i] == s[j])
                return false;
        }
    }

    return true;
}
