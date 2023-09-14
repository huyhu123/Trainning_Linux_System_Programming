#include "input.h"
#include "file_handling.h"

int main() {
    char dir[MAX_FILE_SIZE] = "";
    char name[MAX_FILE_SIZE] = "";

    if (!get_output_name(name)) {
        return 0;
    }

    if (get_input_dir(dir)) {
        search_file_in_dir(dir, name);
    }

    return 0;
}