#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/file.h>

void foo(char *s) {
   char c[8];
   strcpy(c, s);
}

void f1(int argv1_f1, int argv2_f1) {
    int local_variable_1_f1;
    int local_variable_2_f1;
    f2(1, 2); // 0x1002
}

void f2(int argv1_f2, int argv2_f2) {
    int local_variable_2_f1;
    int local_variable_2_f2;
    f3(3, 4); // 0x2004
}

void f3(int argv1_f3, int argv2_f3) {
    int local_variable_3_f1;
    int local_variable_3_f2;
}

int main() {
   foo("ABCDEFGHJKLM"); // 12 char
}