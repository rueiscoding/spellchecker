#include "dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define BUFFERSIZE 4096

// node struct to create the trie
struct node {
	struct node* children[95]; // an array of node pointers
	bool isWord; // true if valid word, false if not
};

// Creates a trie and returns the header to the trie
node* makeDict(char[] filename) {
	// create head of trie
	struct node* head = (struct node*)malloc(sizeof(struct node)); 
	
	char buffer[BUFFERSIZE];
	int numBytesRead = read(filename, buffer, BUFFERSIZE);
	
	if (numBytesRead == -1) {
		// print error if file cannot be read
		printf("ERROR: Could not read file %s.", filename);
	}

	char* bptr = buffer; // ptr to traverse buffer
	struct node* tPtr = head; // ptr to traverse trie

	while ((numBytesRead = read(filename, buffer, BUFFERSIZE)) > 0) {
		for(int i = 0; i < numBytesRead; i++) {

			if(isspace(*bptr)) {
				// if current buffer char is space, set current node isWord to true
				(*tptr).isWord = true;
				tptr = head;
			} else {
				// add character to trie
				int index = (((int)(*bptr)) - 33); // "hash function" that converts char int to index int
				tPtr->children[index] = (struct node*)malloc(sizeof(struct node)); // add child at index
				(tPtr->children[index]).isWord = false;	
			}
	
		}

		bptr = buffer; // reset buffer pointer
	}

	return head;
}


