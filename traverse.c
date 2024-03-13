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

typedef struct FileNode {
    char *path;
    struct FileNode *next;
} FileNode;

FileNode *file_list_head = NULL; // global variable to hold the head of the file list
// label this as 'extern' in readfile.h

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

// creates a linkedlist for filepaths
void add_file_to_list(const char *path){

    // allocate memory for the node and copy path to node
    FileNode *new_node = (FileNode *) malloc(sizeof(FileNode));
    new_node->path = strdup(path);

    // insert node to beginning of list
    new_node->next = file_list_head;
    file_list_head = new_node;
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

// to be called by other files once the file_list is used
void free_file_list(){

    FileNode *current = file_list_head;
    while (current != NULL) {
        FileNode *temp = current;
        current = current->next;

        // free the full_name
        free(temp->path);

        // free the memory for the node
        free(temp);
    }
}

// to test my_files folder
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("requires directory name");
        return 1;
    }

    traverse_directory(argv[1]);

    // printing to test traverse_directory
    FileNode *current = file_list_head;
    while (current != NULL) {
        printf("%s\n", current->path);
        current = current->next;
    }

    free_file_list();

    return 0;
}