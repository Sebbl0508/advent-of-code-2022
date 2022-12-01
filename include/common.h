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
