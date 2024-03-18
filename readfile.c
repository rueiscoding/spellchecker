#include "dictionary.h"
#include "readfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORD_LEN 2048
#define BUFFER_SIZE 4096

// void check_spelling(const char *filename, struct Node* trie_head) {
//     FILE *file = fopen(filename, "r");
//     if (file == NULL) {
//         perror("Error opening file.\n");
//     }

//     char word[MAX_WORD_LEN];
//     int line_num = 1, col_num = 1;
//     int c;
    
//     while ((c = fgetc(file)) != EOF) { // fgetc get next string and includes null terminator
        
//         if (c == '\n') {
//             line_num++;
//             col_num = 1;
//             continue;
//         }

//         if (isWord(trie_head, c)) { // if the word is valid
//             col_num += strlen(c) + 1;  // gets length of  a string or char array without null terminator
//             // +1 for space
//         }
//         else{ // if the word is invalid
//             printf("%s (%d, %d): %s", filename, line_num, col_num, c);
//             col_num += strlen(c) + 1;
//         }

//     }
//     fclose(file);
// }


int main(int argc, char *argv[]) {
    //struct Node* trieheader = makeDict("dictionary.txt");
    //check_spelling("readfiletest.txt", trieheader);

    FILE *file = fopen("readfiletest.txt", "r");
    if (file == NULL) {
        perror("Error opening file.\n");
    }

    char word[MAX_WORD_LEN];
    int line_num = 1, col_num = 1;
    int c;
    
    while ((c = fgetc(file)) != EOF) { // fgetc get next string and includes null terminator
        
        if (c == '\n') {
            line_num++;
            col_num = 1;
            continue;
        }

        col_num += strlen(c) + 1;
        printf("\nword: %s is at line, col: (%d, %d)", c, line_num, col_num);

    }
    fclose(file);


}