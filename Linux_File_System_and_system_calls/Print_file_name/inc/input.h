#ifndef _INPUT_H_
#define _INPUT_H_

#include <errno.h>
#include <ctype.h>
#include "file_handling.h"

#define BUFFER_LENGTH 100
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
 * @param output_name The new output file name.
 */
void change_output_name(char *output_name);

#endif
