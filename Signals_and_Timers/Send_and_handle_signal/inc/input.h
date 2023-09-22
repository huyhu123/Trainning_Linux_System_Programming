#ifndef _INPUT_H_
#define _INPUT_H_

#include <errno.h>
#include <ctype.h>
#include "file_handling.h"

#define BUFFER_LENGTH 200
#define FILE_NAME_MAX 255

/**
 * @brief Changes the output directory.
 *
 * @param dir The new output directory.
 */
bool get_input_dir(char *dir);

/**
 * @brief Changes the output file name.
 *
 * @param file_name The new output file name.
 */
bool get_output_name(char *file_name);

/**
 * @brief Reads a single character of input from the user.
 *
 * This function prompts the user to enter a single character of input and
 * reads it from the standard input stream. The input character is stored in
 * the provided buffer.
 *
 * @param input A pointer to a character buffer where the input character will be stored.
 */
void get_input_char(char *input);

/**
 * @brief Prompts the user to enter a file name and reads it from the standard input stream.
 *
 * This function prompts the user to enter a file name and reads it from the standard input
 * stream. The file name is stored in the provided buffer. If the user enters a valid file
 * name, the function returns true. If the user enters an empty string or encounters an error,
 * the function returns false.
 *
 * @param file_name A pointer to a character buffer where the file name will be stored.
 * @return true if a valid file name was entered, false otherwise.
 */
bool get_file_name(char *file_name);

#endif
