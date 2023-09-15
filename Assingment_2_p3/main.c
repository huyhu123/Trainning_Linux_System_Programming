#include "input.h"
#include "file_handling.h"

int main() {
    char dir[MAX_FILE_SIZE] = {0};
    char name[MAX_FILE_SIZE] = {0};
    char file[MAX_FILE_SIZE] = {0};
    int hard_link = 0;
    int soft_link = 0;

    // Get directory
    if (!get_input_dir(dir)) {
        return 0;
    }

    // Get file name
    if (!get_output_name(name)) {
        return 0;
    }

    // Search file in directory
    if (!search_file_in_dir(dir, name)) {
        printf("File not exist in directory\n");
        return 0;
    }

    // Concatenate dir and file name
    strcpy(file, dir);
    strcat(file, "/");
    strcat(file, name);

    // Create hard link
    hard_link = link(file, "hard_link.txt");
    if (hard_link == 0) {
        printf("Create hard link file %s to %s sucessful\n", "hard_link.txt", file);
    } else {
        printf("False to create hard link\n");
    }

    // Create soft link
    soft_link = symlink(file, "soft_link.txt");
    if (hard_link == 0) {
        printf("Create soft link file %s to %s sucessful\n", "soft_link.txt", file);
    } else {
        printf("False to create soft link\n");
    }

    return 0;
}