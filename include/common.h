#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>


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


#ifndef __USE_MISC
char * strsep(char **sp, char *sep);
#endif
