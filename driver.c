//makes the trie
//gets all the files
//calls readfile method on each file
//frees all the filelist
//frees the trie

#include "traverse.h"
#include "dictionary.h"
#include "readfile.h"
#include "driver.h"

#include <stdio.h>

struct FileNode { //idk y but its making me declare it again..sUS
    char *path;
    struct FileNode *next;
};

int main(int argc, char *argv[]) {

    char *path_to_dictionary = argv[1]; // path to dictionary

    int array_size = argc - 2;
    if(array_size <=0){
        perror("Not enough arguments");
    }

    struct Node* trie_header = makeDict(path_to_dictionary);

    int count = 0;
    for (int i = 2; i < argc; i++) {
        printf("%d arg at\n", i);
        const char *traverse_this = argv[i];
        struct FileNode *current = command_line_traverse(traverse_this);
        while (current != NULL) {
            //printf("%s\n", current->path);
            check_spelling(current->path, trie_header);
            current = current->next;
        }

        free_file_list();

    }


    freeDict(trie_header);
}