#include "traverse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/**
 * Traverse.c performs recursive directory traversal given a 
 * directory name and returns a list of text files.
 */

struct FileNode {
    char *path;
    struct FileNode *next;
};

struct FileNode *file_list_head = NULL;

// valid files end with '.txt', ignore files starting with '.'
int is_valid_file(const char *filename) {

    // if filename begins with '.'
    if(filename[0] == '.'){
        return 0; // file is invalid
    }

    // last occurance of '.'
    const char *file_extension = strrchr(filename, '.');

    // if file_extension is null or does not end in '.txt'
    if (file_extension == NULL || strcmp(file_extension, ".txt") != 0)
        return 0; // file is invalid
    
    return 1; // file is valid
}

// valid directories do not start with "."
int is_valid_directory(char *directory){

    return (directory[0] == '.') ? 0 : 1;
}

// creates a linkedlist for filepaths. returns pointer to beginning of filepath
void add_file_to_list(const char *path){

    // allocate memory for the node and copy path to node
    struct FileNode *new_node = (struct FileNode *) malloc(sizeof(struct FileNode));
    new_node->path = strdup(path);

    // insert node to beginning of list
    new_node->next = file_list_head;
    file_list_head = new_node;
}

// if file name matches the base filename
// example: filename as 'helloworld.txt' and base_filename as 'helloworld'
int matches_base_filename(const char *filename, const char *base_filename) {

    if (strcmp(filename, base_filename) == 0){
        return 1;
    }

    size_t len = strlen(base_filename);
    return strncmp(filename, base_filename, len) == 0 && filename[len] == '.';
}

// recursively finds all .txt files in a directory, returns a list
void traverse_directory(const char *directory) {

    struct dirent *entry; // pointer for directory entry

    DIR *dir = opendir(directory); // returns pointer of DIR type
    if (dir == NULL) {
        perror("Could not open current directory");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {

        // calculate size; +1 for '/' and +1 for null terminator
        size_t full_name_len = strlen(directory) + 1 + strlen(entry->d_name) + 1;
        
        // allocate memory for full_name, and concatenate current path with entry name
        char *full_name = (char *)malloc(full_name_len); //do i check if malloc fails??
        snprintf(full_name, full_name_len, "%s/%s", directory, entry->d_name); // hardcoding '/'

        // saves entry status in path_stat. use path stat to check if file or dir
        struct stat path_stat;
        stat(full_name, &path_stat); 

        if (S_ISDIR(path_stat.st_mode)){ // is a directory
            
            if(is_valid_directory(entry->d_name)){
                traverse_directory(full_name);
            }

        }else if (S_ISREG(path_stat.st_mode)) { // is a regular file

            if(is_valid_file(entry->d_name)){
                add_file_to_list(full_name);
            }
        }
    }

    closedir(dir);
}

/**
 * command line arguments don't need to follow directory traversal rules of
 * valid-invalid files or directories
 * "the requirements to ignore files beginning with a period and not ending with “.txt”
 * only apply to directory traversal, not to files in the argument list."
 */
struct FileNode* command_line_traverse(const char *argument){
    char full_filename[FILENAME_MAX];
    DIR *dir;
    struct dirent *entry;

    char cwd[2048]; // assuming max length
    getcwd(cwd, sizeof(cwd)); // check in present directory for a match
    dir = opendir(cwd);
    if (dir == NULL) {
        perror("Unable to open directory");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (matches_base_filename(entry->d_name, argument)) { // found matching file or directory
            strcpy(full_filename, entry->d_name);
            struct stat path_stat;
            stat(full_filename, &path_stat);

            if (S_ISREG(path_stat.st_mode)){ 
                add_file_to_list(full_filename);
                return file_list_head; 
            }else if(S_ISDIR(path_stat.st_mode)){
                traverse_directory(full_filename);
                return file_list_head;
            }
            break;
        }
    }
    return NULL;
}

// to be called by other files once the file_list is used
void free_file_list(){

    struct FileNode *current = file_list_head;
    while (current != NULL) {
        struct FileNode *temp = current;
        current = current->next;

        // free the full_name
        free(temp->path);

        // free the memory for the node
        free(temp);
    }
    file_list_head = NULL;
}

// to test my_files folder
// int main(int argc, char *argv[]) {
//     if (argc != 2) {
//         printf("requires directory name");
//         return 1;
//     }

//     //traverse_directory(argv[1]);
//     struct FileNode *current = command_line_traverse(argv[1]);

//     while (current != NULL) {
//         printf("%s\n", current->path);
//         current = current->next;
//     }

//     free_file_list();

//     return 0;
// }