#include "dictionary.h"
#include "readfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for read()
#include <fcntl.h> // for open()
#include <ctype.h> // for isspace()

#define MAX_WORD_LEN 2048
#define BUFFER_SIZE 4096

void check_spelling(const char *filename, struct Node* trie_head) {
    int file_desc, n_bytes;
    char buffer[MAX_WORD_LEN];
    char word[MAX_WORD_LEN];
    int row_num = 0, col_num = 0;
    int i = 0;

    file_desc = open(filename, O_RDONLY);
    if (file_desc < 0) {
        perror("Error opening file");
    }

    while ((n_bytes = read(file_desc, buffer, MAX_WORD_LEN)) > 0) {

        for (i = 0; i < n_bytes; i++) {

            if (buffer[i] == '\n') {
                row_num++;
                col_num = 0;
            } 
            else if (isspace(buffer[i])) {
                col_num++;
            } else {
                
                word[0] = buffer[i];
                int j = 1;
                while (i + j < n_bytes && !isspace(buffer[i + j]) && j < MAX_WORD_LEN - 1) {
                    word[j] = buffer[i + j];
                    j++;
                }
                word[j] = '\0'; //null term
                if (!isWord(trie_head, word)){
                    printf("\n%s (%d, %d): %s", filename, row_num + 1, col_num + 1, word);
                }
                col_num+= j;
                i += j - 1;
            }
        }
    }

    close(file_desc);

}



// int main(int argc, char *argv[]) {
//     struct Node* trieheader = makeDict("dictionary.txt");
//     check_spelling("readfiletest.txt", trieheader);

//     freeDict(trieheader);

// }