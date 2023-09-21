#ifndef _FILE_HANDLING_H_
#define _FILE_HANDLING_H_

#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#define MAX_FILE_SIZE 255
#define MAX_LINE_SIZE 256
#define BUFFER_LEN    4026

/**
 * @brief Reads the contents of a file into a string.
 *
 * This function reads the contents of the file specified by the `dir` parameter and stores it in the `text` parameter.
 *
 * @param dir The directory path of the file to read.
 * @param text Pointer to a buffer where the contents of the file will be stored.
 */
void read_from_file(char *dir, char *text);

/**
 * @brief Writes a string to a file.
 *
 * This function writes the contents of the `text` parameter to the file specified by the `dir` parameter.
 *
 * @param dir The directory path of the file to write.
 * @param text The string to write to the file.
 */
void write_to_file(char *dir, char *text);

/**
 * @brief Checks if a directory is empty.
 *
 * This function checks if the directory specified by the `file_name` parameter is empty or not.
 *
 * @param file_name The name of the directory to check.
 * @return True if the directory is empty, false otherwise.
 */
bool check_empty_dir(const char file_name[]);

/**
 * @brief Checks if a file exists.
 *
 * This function checks if the file specified by the `file_name` parameter exists or not.
 *
 * @param file_name The name of the file to check.
 * @return 0 if the file exists, -1 otherwise.
 */
int check_file_exist(const char file_name[]);

/**
 * @brief Retrieves the current working directory.
 *
 * This function returns a pointer to a string representing the current working directory.
 *
 * @return A pointer to a string containing the current working directory.
 */
char *get_current_directory();

/**
 * @brief Checks if a directory exists.
 *
 * This function checks if the directory specified by the `path` parameter exists.
 *
 * @param path The path of the directory to check.
 * @return True if the directory exists, false otherwise.
 */
bool directory_exists(const char *path);

/**
 * @brief Checks if a file name is valid.
 *
 * This function checks if the file name specified by the `file_name` parameter is valid.
 * Valid file names do not contain any illegal characters or reserved names.
 *
 * @param file_name The name of the file to check.
 * @return True if the file name is valid, false otherwise.
 */
bool is_validfile_name(const char *file_name);

/**
 * @brief Creates a new file name based on input and output parameters.
 *
 * This function creates a new file name by combining the input file name, output name, and output directory.
 * The new file name is stored in the `new_file_name` parameter.
 *
 * @param new_file_name Pointer to a character buffer where the new file name will be stored.
 * @param file_name The name of the input file.
 * @param output_name The desired name for the output file.
 * @param dir The directory path where the output file will be created.
 */
void create_new_file_name(char *new_file_name, char *file_name, char *output_name, char *dir);

/**
 * \brief Lists all the files in a directory.
 *
 * This function takes a directory path as input and lists all the files
 * (excluding subdirectories) present in that directory.
 *
 * \param dir A pointer to a character array containing the path of the directory.
 * \note The directory path must be a valid path.
 *
 * \return None.
 */
void list_file_in_dir(char *dir);

/**
 * @brief Searches for a file with the given name in the specified directory.
 *
 * This function searches for a file with the given name in the specified directory.
 * If the file is found, the function returns true. Otherwise, it returns false.
 *
 * @param dir The directory to search in.
 * @param name The name of the file to search for.
 * @return true if the file is found, false otherwise.
 */
bool search_file_in_dir(char *dir, char *name);

/**
 * @brief Prompts the user to enter a name for the output file and reads it from the standard input stream.
 *
 * This function prompts the user to enter a name for the output file and reads it from the standard input
 * stream. The file name is stored in the provided buffer. If the user enters a valid file name, the function
 * returns true. If the user enters an empty string or encounters an error, the function returns false.
 *
 * @param file_name A pointer to a character buffer where the file name will be stored.
 * @return true if a valid file name was entered, false otherwise.
 */
bool get_output_name(char *file_name);

/**
 * @brief Creates a hard link between the source file and the destination file.
 *
 * This function creates a hard link between the source file and the destination file.
 * If the operation is successful, the function returns true. Otherwise, it returns false.
 *
 * @param dest_file The name of the destination file.
 * @param src_file The name of the source file.
 * @return true if the operation is successful, false otherwise.
 */
void create_hard_link(char *dest_file, char *src_file);

/**
 * @brief Creates a soft link between the source file and the destination file.
 *
 * This function creates a soft link between the source file and the destination file.
 * If the operation is successful, the function returns true. Otherwise, it returns false.
 *
 * @param dest_file The name of the destination file.
 * @param src_file The name of the source file.
 * @return true if the operation is successful, false otherwise.
 */
void create_soft_link(char *dest_file, char *src_file);

#endif
