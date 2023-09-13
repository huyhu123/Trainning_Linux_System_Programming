#include "input.h"
#include "file_handling.h"

int main() {
    char dir[MAX_FILE_SIZE];

    if(get_input_dir(dir)) {
        list_file_in_dir(dir);
    }

    return 0;
}