#include "input.h"
#include "file_handling.h"

int main() {
    char filename[FILE_NAME_MAX] = "";
    char text[BUFFER_LENGTH] = "";

    if (!get_file_name(filename)) {
        return 0;
    }

    printf("Enter text to write to file: ");
    get_input_char(text);
    write_to_file(filename, text);

    return 0;
}