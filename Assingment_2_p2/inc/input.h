#ifndef _INPUT_H_
#define _INPUT_H_

#include <errno.h>
#include <ctype.h>
#include "file_handling.h"

#define BUFFER_LENGTH       200
#define FILE_NAME_MAX       255

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

void get_input_char(char *input);

bool get_file_name(char *file_name);

#endif
