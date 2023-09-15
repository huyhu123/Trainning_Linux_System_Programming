#include "input.h"

// Clear stdin
static int clean_stdin()
{
    int c;
    while (c = getchar() != '\n' && c != EOF);
    return 1;
}

// Check if buffer if overflow
static bool check_input_buffer(char input[])
{
    if (strcspn(input, "\n") == FILE_NAME_MAX - 1) {
        clean_stdin();
        return true;
    }
    return false;
}

bool is_integer(const char *buffer, int* dest, int min, int max) {
    if (buffer == NULL) {
        return false;
    }

    char *endptr;
    errno = 0; // Reset errno before calling strtol
    long result = strtol(buffer, &endptr, 10);
    
    // Check for conversion errors
    if (buffer == endptr || errno == ERANGE || result > max || result < min) {
        return false; // Conversion error or invalid
    }

    // Look at trailing text
    while (isspace((unsigned char)*endptr)) {
        endptr++;
    }

    // Check if there is any trailing text after the number
    if (*endptr != '\0') {
        return false; // Trailing text present
    }

    *dest = (int)result; // Assign converted value to the destination variable
    return true; // Successful conversion
}

void get_input_int(int *value, int min, int max) 
{
    char buffer[BUFFER_LENGTH] = "";
    char *endptr;

    // Get input
    fgets(buffer, sizeof(buffer), stdin);
 
    while(!is_integer(buffer, value, min, max)) {
        //Handle invalid input
        printf("Invalid input or input too large: ");
        fgets(buffer, sizeof(buffer), stdin);
    }
}

void get_input_char(char *input) 
{
    fgets(input, FILE_NAME_MAX, stdin);

    // Check if input is valid
    while (check_input_buffer(input) == 1) {
        printf("\tInput too long, enter again (or e to exit): ");
        fgets(input, FILE_NAME_MAX, stdin);
    }
    // Strip line break at the end of input
    input[strcspn(input, "\n")] = 0;
}


// Get file name input from keybroad and check if file name is valid 
// If press e return false
bool get_file_name(char *file_name)
{
    char pre_file_name[MAX_FILE_SIZE] = "";
    strcpy(pre_file_name, file_name);

    // Get file name
    printf("Enter file name (or e to exit): ");
    get_input_char(file_name);
    if (strcmp(file_name, "e") == 0) {
        strcpy(file_name, pre_file_name);
        return false;
    }

    // Check if file exist
    while (!check_file_exist(file_name)) {
        printf("File not exist, enter again (or e to exit): ");
        get_input_char(file_name);
        if (strcmp(file_name, "e") == 0) {
            strcpy(file_name, pre_file_name);
            return false;
        }
    }

    return true;
}

// Change name of a char * and check if name is a valid file name
bool get_output_name(char *file_name)
{
    char pre_file_name[FILE_NAME_MAX] = "";
    strcpy(pre_file_name, file_name);

    // Get output file name
    printf("Enter new output file name (or e to exit): ");
    get_input_char(file_name);
    if (strcmp(file_name, "e") == 0) {
        strcpy(file_name, pre_file_name);
        return false;
    }

    // Check if name valid
    while (!is_validfile_name(file_name)) {
        printf("Invalid file name (contain '\\/:*?\"<>|', too long or have space, '.' at start or end)\n Enter again (or e to exit): ");
        get_input_char(file_name);
        if (strcmp(file_name, "e") == 0) {
            strcpy(file_name, pre_file_name);
            return false;
        }
    }

    return true;
}

// Change directory and check if dir is exist
bool get_input_dir(char *dir)
{
    char pre_output_name[FILE_NAME_MAX] = "";
    strcpy(pre_output_name, dir);

    // Get output dir
    printf("Enter a directory (or e to exit): ");
    get_input_char(dir);
    if (strcmp(dir, "e") == 0) {
        strcpy(dir, pre_output_name);
        return false;
    }

    // Check if dir exist
    while (!directory_exists(dir)) {
        printf("Directory not exist, enter again (or e to exit): ");
        get_input_char(dir);
        if (strcmp(dir, "e") == 0) {
            strcpy(dir, pre_output_name);
            return false;
        }
    }

    return true;
}

