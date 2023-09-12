#ifndef _INPUT_H_
#define _INPUT_H_

#include <errno.h>
#include <ctype.h>
#include "file_handling.h"

#define BUFFER_LENGTH       100
#define FILE_NAME_MAX       255

/**
 * @brief Changes the output directory.
 *
 * @param output_dir The new output directory.
 */
void change_output_dir(char *output_dir);

/**
 * @brief Changes the output file name.
 *
 * @param output_name The new output file name.
 */
void change_output_name(char *output_name);

#endif
