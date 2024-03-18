#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // for read()
#include <fcntl.h> // for open()
#include <ctype.h> // for isspace()

#define BUFFERSIZE 4096
#define NUMPOSCHARS 95 // max number of possible characters

// node struct to create the trie
struct Node {
	char character;
	struct Node* children[NUMPOSCHARS]; // an array of nodes
	int isWord; // true if valid word, false if not
};

// Creates a trie and returns the header to the trie
struct Node* makeDict(char filename[]) {
	// create head of trie
	struct Node* head = (struct Node*)malloc(sizeof(struct Node)); 
	
	for (int i = 0;  i < NUMPOSCHARS; i++) {
		// initialize children to NULL
		head->children[i] = NULL;
	}

	char buffer[BUFFERSIZE];
	int fd = open(filename, O_RDONLY);
	int numBytesRead = read(fd, buffer, BUFFERSIZE);
	
	if (numBytesRead == -1) {
		// print error if file cannot be read
		printf("ERROR: Could not read file %s.", filename);
	}

	char* bptr = buffer; // ptr to traverse buffer
	struct Node* tptr = head; // ptr to traverse trie

	do {
		// keep reading until end of file

		for(int i = 0; i < numBytesRead; i++) {
			// read buffer
			
			if (*bptr == '\n') {
				// if buffer pointer is next line character, set tptr isWord to true
				tptr->isWord = 1;
				tptr = head;

			} else {
				// add character to trie
				int index = hash(*bptr); // get index of character
				
				if (index > 127 || index < 0) {
					return 0; // return false if character is outside of valid ASCII range
				}

				if (tptr->children[index] == NULL) {
					// if child does not exist, create it
					tptr->children[index] = (struct Node*)malloc(sizeof(struct Node)); // add child at index
					tptr->isWord = 0;
					tptr->character = *bptr; 
				}
				
				// continue trie
				tptr = tptr->children[index];	
			}
			
			bptr++;
	
		}

		bptr = buffer; // reset buffer pointer
	} while ((numBytesRead = read(fd, buffer, BUFFERSIZE)) > 0);
	
	

	close(fd);

	return head;

}

// "hash function" that convers char int to index int
int hash(char c) {
	return (((int)c) - 33);
}

/*
 * Frees the trie.
 */
void freeDict(struct Node* head) {
	if ((*head).isWord) {
		
		free(head);
		
		return;
	} else {
		
		for (int i; i < NUMPOSCHARS; i++) {
			// free children
			if (head->children[i] != NULL) {
				struct Node* ptr = head->children[i];
				freeDict(ptr);
			}	
		}
	}
}

/*
 * Checks if given string is a word.
 */
int isWord(struct Node* head, char word[]) {
	char* wptr = word; // word pointer
	struct Node* tptr = head; // trie pointer
	int isWord = 0;

	while ((int)(*wptr) > 127 || (int)(*wptr) < 0 || (int)(*wptr) == ' ' || (int)(*wptr) == '\'' || (int)(*wptr) == '"' || (int)(*wptr) == '{' || (int)(*wptr) == '}' || (int)(*wptr) == '[' || (int)(*wptr)  == ']' || (int)(*wptr)  == '(' || (int)(*wptr)  == ')') {
		// ignore invalid characters at beginning of word
        	wptr++;
	}

	while ((*wptr) != '\0') {
		// while the next character is not the end of string character, traverse trie
		int index = hash(*wptr);

		if ((int)(*wptr) > 127 || (int)(*wptr) < 0) {
			// ignore any characters outside of valid ASCII
			wptr++;
			continue;

		} else 

		if (*wptr == '-') {
			if (tptr->isWord == 1) {
				// if string before - is a word, then continue and set tptr to head
				tptr = head;
				wptr++;
				continue;
			}
		} else

		if (tptr->children[index] == NULL) {
			if (tptr->isWord == 1) {
				// run if we reach leaf node before processing entire word

				isWord = 1; 

				while ((*wptr) != '\0') {
					// continue to traverse string
				
					if (isalpha(*wptr)) {
						// if there are any letters after valid word, word is invalid	
						isWord = 0;
					}
				}

			} else {
				return 0;
			}

			return isWord;
		} 

		tptr = tptr->children[index];
		wptr++;
	}

	if ((*tptr).isWord) {
		isWord = 1;
	}

	return isWord;
}
