#include <stdio.h>
#include <stdlib.h>
#include "traverse.h"


// checks the traversal of a list of files n directories
// does not require full path

struct FileNode {
    char *path;
    struct FileNode *next;
};

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("requires directory name");
        return 1;
    }

    //traverse_directory(argv[1]);
    struct FileNode *current = command_line_traverse(argv[1]);

    while (current != NULL) {
        printf("%s\n", current->path);
        current = current->next;
    }

    free_file_list();

    return 0;
}