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

void get_input_char(char *input);

void get_input_float(float *value, float min, float max);

void get_input_int(int *value, int min, int max);

#endif
