#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define IS_DIGIT(c) (c >= '0' && c <= '9')
#define CHAR_TO_U8(c) ((uint8_t)c - 48)

/// @brief Reads the file at the supplied path to a string.
/// @warning This function crashes on all errors
/// @param path Path to the file
/// @return File contents as string
char* read_file_to_string(char* path);


/// @brief Copies the supplied string to a new string
/// @param s Old string
/// @return Newly allocated string
char* copy_str(char* s);


/// @brief Comparison function for `qsort`
/// @param in_a uint32_t pointer
/// @param in_b uint32_t pointer
int compare_u32(const void* in_a, const void* in_b);

size_t count_lines(const char* s);

#ifndef __USE_MISC
char * strsep(char **sp, char *sep);
#endif
