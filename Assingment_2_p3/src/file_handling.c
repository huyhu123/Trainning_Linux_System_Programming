#include "file_handling.h"

void read_from_file(char *dir, char *text)
{
    FILE *fp;
    fp = fopen(dir, "r");
    if (fp != NULL) {
        char ch;
        int index = 0;
        while ((ch = fgetc(fp)) != EOF) {
            if (ch == '\n') {
                text[index++] = ' ';
                text[index++] = '|'; // Use '|' to indicate line breaks
                text[index++] = ' ';
            } else {
                text[index++] = ch;
            }
        }
        text[index] = '\0'; // Null-terminate the string
        fclose(fp);
    }
}

void write_to_file(char *dir, char *text)
{
    FILE *fp;
    fp = fopen(dir, "w");
    fprintf(fp, "%s", text);
    fclose(fp);
}

// Check empty dir
bool check_empty_dir(const char file_name[])
{
    int index = 0;

    for (index; index < strlen(file_name); index++) {
        if (file_name[index] != '\\' && file_name[index] != '/' && file_name[index] != '.') {
            return true;
        }
    }

    return false;
}

// Check if file exist
int check_file_exist(const char file_name[])
{
    if (file_name == NULL) {
        return 0;
    }

    if (!check_empty_dir(file_name)) {
        return 0;
    }

    FILE *file;
    if ((file = fopen(file_name, "r"))) {
        fclose(file);
        return 1;
    }
    return 0;
}

// Get current dir
char *get_current_directory() 
{
    char *buffer = (char*)malloc(sizeof(char) * 4096);

    if (getcwd(buffer, 4096) == NULL) {
        perror("Error getting current directory");
        free(buffer);
        return NULL;
    }

    return buffer;
}

// Check if directory exist
bool directory_exists(const char *path) 
{
    if (access(path, F_OK) != -1) {
        return true; // directory exists
    } else {
        return false; // directory does not exist
    }
}

// Check if file name is valid
bool is_validfile_name(const char *file_name) 
{
    // Check if the file name is empty or exceeds the maximum length
    if (strlen(file_name) == 0 || strlen(file_name) > MAX_FILE_SIZE) {
        return false;
    }
    
    // Check if the file name contains any invalid characters
    const char *invalid_chars = "\\/:*?\"<>|";
    for (int i = 0; i < strlen(invalid_chars); i++) {
        if (strchr(file_name, invalid_chars[i]) != NULL) {
            return false;
        }
    }
    
    // Check if the file name starts or ends with a space or a period
    if (file_name[0] == ' ' || file_name[0] == '.' || 
        file_name[strlen(file_name)-1] == ' ' || file_name[strlen(file_name)-1] == '.') {
        return false;
    }
    
    return true;
}

// Create new file name with the directory and extention of old file name 
void create_new_file_name(char *new_file_name, char *file_name, char *output_name, char *dir)
{
    // Get file extension
    char *ext = strrchr(file_name, '.');
    if (!ext) {
        ext = "";
    } else {
        ext = ext + 1;
    }

    // Make new file name
    strcpy(new_file_name, dir);
    strcat(new_file_name, "/");
    strcat(new_file_name, output_name);
    strcat(new_file_name, ".");
    strcat(new_file_name, ext);
}

char *convert_file_type(char type)
{
    switch (type) {
        case 'd':
            return "directory";
        case 'b':
            return "block file";
        case 'l':
            return "symbolic file";
        case 'c':
            return "charactor device file";
        case 's':
            return "socket file";
        case 'p':
            return "pipe file";
        case '-':
            return "regular file";
        default:
            return "unknown";
    }
}

void extract_file_info(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return;
    }

    printf("\n%-20s %-20s\n", "File name", "File type");
    printf("--------------------------------------\n");
    char line[MAX_LINE_SIZE] = "";
    bool skip_first_line = true; // Flag for skipping the first line
    while (fgets(line, sizeof(line), file)) {
        if (skip_first_line) {
            skip_first_line = false;
            continue; // Skip the first line
        }

        char* token = strtok(line, " \n");
        if (token != NULL) {
            char* last_word = token; // Initialize last_word with the first token
            while (token != NULL) {
                last_word = token;
                token = strtok(NULL, " \n");
            }
            printf("%-20s %-20s\n", last_word, convert_file_type(line[0]));
        }
    }

    fclose(file);
}

void list_file_in_dir(char *dir) 
{
    // Create and run command "ls -l 'directory' > temp.txt"
    char command[100] = "";
    strcpy(command, "ls -l");
    strcat(command, " ");
    strcat(command, dir);
    strcat(command, " > temp.txt");
    system(command);

    // Get file infomation
    extract_file_info("temp.txt");

    // Delete temp.txt
    system("rm -rf temp.txt");

    return;
}

int is_name_in_dir(const char *filename, const char *name) 
{
    FILE *file = fopen(filename, "r");
    char line[MAX_LINE_SIZE] = {0};

    if (file == NULL) {
        printf("Could not open the file.\n");
        return 0;
    }

    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0'; // Remove newline character
        
        if (strcmp(line, name) == 0) {
            fclose(file);
            return 1;
        }
    }
    
    fclose(file);
    return 0;
}

bool search_file_in_dir(char *dir, char *name)
{
    char file[MAX_FILE_SIZE] = {0};
    char command[MAX_LINE_SIZE] = {0};
    bool return_value = false;

    // Create and run command "ls 'directory' > temp.txt"
    strcpy(command, "ls");
    strcat(command, " ");
    strcat(command, dir);
    strcat(command, " > temp.txt");
    system(command);

    // Check if file exist in dir
    if (is_name_in_dir("temp.txt", name)) {
        return_value = true;
    }

    // Delete temp.txt
    system("rm -rf temp.txt");

    return return_value;
}

