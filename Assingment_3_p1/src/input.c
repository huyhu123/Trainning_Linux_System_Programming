#include "input.h"

// Clear stdin
static int clean_stdin()
{
    int c;
    while (c = getchar() != '\n' && c != EOF)
        ;
    return 1;
}

// Check if buffer if overflow
static bool check_input_buffer(char input[])
{
    if (strcspn(input, "\n") == FILE_NAME_MAX - 1)
    {
        clean_stdin();
        return true;
    }
    return false;
}

bool is_integer(const char *buffer, int *dest, int min, int max)
{
    char *endptr;
    errno = 0; // Reset errno before calling strtol
    long result = 0;

    if (buffer == NULL)
    {
        return false;
    }

    result = strtol(buffer, &endptr, 10);

    // Check for conversion errors
    if (buffer == endptr || errno == ERANGE || result > max || result < min)
    {
        return false; // Conversion error or invalid
    }

    // Look at trailing text
    while (isspace((unsigned char)*endptr))
    {
        endptr++;
    }

    // Check if there is any trailing text after the number
    if (*endptr != '\0')
    {
        return false; // Trailing text present
    }

    *dest = (int)result; // Assign converted value to the destination variable
    return true;         // Successful conversion
}

bool is_float(const char *buffer, float *dest, float min, float max)
{
    char *endptr;
    errno = 0; // Reset errno before calling strtof
    float result = 0.0;

    if (buffer == NULL)
    {
        return false;
    }

    result = strtof(buffer, &endptr);

    // Check for conversion errors
    if (buffer == endptr || errno == ERANGE || result > max || result < min)
    {
        return false; // Conversion error or invalid
    }

    // Look at trailing text
    while (isspace((unsigned char)*endptr))
    {
        endptr++;
    }

    // Check if there is any trailing text after the number
    if (*endptr != '\0')
    {
        return false; // Trailing text present
    }

    *dest = result; // Assign converted value to the destination variable
    return true;    // Successful conversion
}

void get_input_int(int *value, int min, int max)
{
    char buffer[BUFFER_LENGTH] = "";
    char *endptr;

    // Get input
    fgets(buffer, sizeof(buffer), stdin);

    while (!is_integer(buffer, value, min, max))
    {
        // Handle invalid input
        printf("Invalid input or input too large: ");
        fgets(buffer, sizeof(buffer), stdin);
    }
}

void get_input_float(float *value, float min, float max)
{
    char buffer[BUFFER_LENGTH] = "";
    char *endptr;

    // Get input
    fgets(buffer, sizeof(buffer), stdin);

    while (!is_float(buffer, value, min, max))
    {
        // Handle invalid input
        printf("Invalid input or input too large: ");
        fgets(buffer, sizeof(buffer), stdin);
    }
}

void get_input_char(char *input)
{
    fgets(input, FILE_NAME_MAX, stdin);

    // Check if input is valid
    while (check_input_buffer(input) == 1)
    {
        printf("\tInput too long, enter again (or e to exit): ");
        fgets(input, FILE_NAME_MAX, stdin);
    }
    // Strip line break at the end of input
    input[strcspn(input, "\n")] = 0;
}