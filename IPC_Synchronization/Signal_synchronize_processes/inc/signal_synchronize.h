#ifndef _SIGNAL_SYNCHRONIZE_H_
#define _SIGNAL_SYNCHRONIZE_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include <stdbool.h>

#define OUTPUT_FILE "output.txt"
#define LINE_LENGTH 255

void process_a();

#endif
