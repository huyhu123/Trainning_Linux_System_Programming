#include "input.h"
#include "file_handling.h"

int main()
{
    char dir[MAX_FILE_SIZE] = {0};
    char name[MAX_FILE_SIZE] = {0};
    char file[MAX_FILE_SIZE] = {0};

    // Get directory
    if (!get_input_dir(dir))
    {
        return 0;
    }

    // Search file in directory
    do
    {
        // Get file name
        if (!get_output_name(name))
        {
            return 0;
        }
    } while (!search_file_in_dir(dir, name));

    // Concatenate dir and file name
    strcpy(file, dir);
    strcat(file, "/");
    strcat(file, name);

    // Create hard link
    create_hard_link(file, "hard_link.txt");

    // Create soft link
    create_soft_link(file, "soft_link.txt");

    return 0;
}