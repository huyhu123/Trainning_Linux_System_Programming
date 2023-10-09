#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#define BUFFER_LENGTH 200
#define FILE_NAME_MAX 255

/**
 * @brief Reads a single character from standard input and stores it in the given pointer.
 * 
 * @param input Pointer to a character variable where the input will be stored.
 */
void get_input_char(char *input);

/**
 * @brief Reads a float value from standard input and stores it in the given pointer, ensuring it falls within the specified range.
 * 
 * @param value Pointer to a float variable where the input will be stored.
 * @param min Minimum allowed value for the input.
 * @param max Maximum allowed value for the input.
 */
void get_input_float(float *value, float min, float max);

/**
 * @brief Reads an integer value from standard input and stores it in the given pointer, ensuring it falls within the specified range.
 * 
 * @param value Pointer to an integer variable where the input will be stored.
 * @param min Minimum allowed value for the input.
 * @param max Maximum allowed value for the input.
 */
void get_input_int(int *value, int min, int max);

bool is_integer(const char *buffer, int *dest, int min, int max);

#endif
