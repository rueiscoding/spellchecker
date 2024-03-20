#include <stdio.h>
#include <stdlib.h>
#include "dictionary.h"
#include "readfile.h"

// checks spelling for single file
// for debugging and testing
// requires full path

int main(int argc, char *argv[]) {
    if(argc < 3){
        perror("Not enough arguments");
    }
    
    struct Node* trieheader = makeDict(argv[1]);
    check_spelling(argv[2], trieheader);

    freeDict(trieheader);
}