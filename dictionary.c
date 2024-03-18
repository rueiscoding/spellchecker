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
	struct Node* tptr = head; // ptr to traverse trie. Handles exact capitalization.

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
	char* wptr = word; // original word pointer
	struct Node* tptr = head; // trie pointer
	int isWord = 0;
	int isCap[2048]; // -1 if don't need to check cap, 0 if isn't capitalized, 1 if is capitalized, -2 if end of word
	int* isCapPtr = isCap;

	// check spelling only
	
	while ((int)(*wptr) > 127 || (int)(*wptr) < 0 || (int)(*wptr) == ' ' || (int)(*wptr) == '\'' || (int)(*wptr) == '"' || (int)(*wptr) == '{' || (int)(*wptr) == '}' || (int)(*wptr) == '[' || (int)(*wptr)  == ']' || (int)(*wptr)  == '(' || (int)(*wptr)  == ')') {
		// ignore invalid characters at beginning of word
        	*isCapPtr = -1; // don't need to check cap
		isCapPtr++;
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
				*isCapPtr = -1; // don't need to check cap
				isCapPtr++;
				tptr = head;
				wptr++;
				continue;
			}
		} else

		if (tptr->children[index] == NULL) {
			int upper;
			if (isalpha(*wptr) && isupper(*wptr)) {
				// if alphabetical, switch case if not matching
				index = hash(tolower(*wptr));
				upper = 0; // lowercase
			} else 
			if (isalpha(*wptr) && islower(*wptr)) {
				// if alphabetical, switch case if not matching
				index = hash(toupper(*wptr));
				upper = 1; // uppercase
			}

			if (tptr->children[index] != NULL) {
				// if it matches, then spelling matches. Continue.
				if (upper) {
					*isCapPtr = 1; // is capitalized
				} else {
					*isCapPtr = 0; // isn't capitalized
				}

				isCapPtr++;

				tptr = tptr->children[index];
				wptr++;
				continue;
			}

			// if still does not match, handle

			if (tptr->isWord == 1) {
				// run if we reach leaf node before processing entire word

				isWord = 1; 

				while ((*wptr) != '\0') {
					// continue to traverse string
				
					if (isalpha(*wptr)) {
						// if there are any letters after valid word, word is invalid	
						isWord = 0;
					}

					*isCapPtr = -1; // don't need to check cap
					isCapPtr++;
				}
				
				*isCapPtr = -2; // end of word

			} 

		} 

		tptr = tptr->children[index]; // check next character
		wptr++;
	}

	if ((*tptr).isWord) {
		isWord = 1;
	}

	// check capitalization if spelling is correct
	if (isWord == 1) {
		isCapPtr = isCap; // pointer to capitalization array
		wptr = word;
	
		// traverse word
		if (*isCapPtr == -1) {
			// don't need to check cap
			isCapPtr++;	
			wptr++;
		} else 
		if (*isCapPtr == 0 && isupper(*wptr)) {
			// if dict is not cap but word is cap, expect all uppercase
			while (*wptr != '\0') {
				// read to end of word
				if (*isCapPtr == -1) {
					// don't need to check cap
					isCapPtr++;
					wptr++;
					continue;
				} else {
					if (!isupper(*wptr)) {
						return 0; // return false if not all uppercase
					}
				}

				isCapPtr++;
				wptr++;
			}

			return 1; // return true if all uppercase
		} else 
		if (*isCapPtr == 0 && !isupper(*wptr)) {
			// if dict is not cap and word is not cap, expect all lowercase except for dict cap characters
			while (*wptr != '\0') {
				// read to end of word
				if (*isCapPtr == -1) {
					// don't need to check cap
					isCapPtr++;
					wptr++;
					continue;
				} else 
				if (*isCapPtr == 0) {
					// if dict is lowercase, word should be lowercase
					if (isupper(*wptr)) {
						return 0; // return false if not lowercase	
					}
				} else
				if (*isCapPtr == 1) {
					// if dict is capital, word should be capital
					if (!isupper(*wptr)) {
						return 0; // return false if not uppercase
					}
				}

				isCapPtr++;
				wptr++;
			}
		} else 
		if (*isCapPtr == 1 && isupper(*wptr)) {
			// if dict is cap and word is cap, continue
			isCapPtr++;
			wptr++;

			while (*wptr != '\0') {
				if (*isCapPtr == -1) {
					// don't need to check cap
					isCapPtr++;
					wptr++;
					continue;
				} else
				if (*isCapPtr == 0) {
					// if dict is not capitalized
					if (isupper(*wptr)) {
						// if word is cap, expect capital
						while (*wptr != '\0') {
							if (*isCapPtr == -1) {
								// don't need to check cap
								isCapPtr++;
								wptr++;
								continue;
							} else {
								// check for capital
								if (!isupper(*wptr)) {
									return 0; // if not cap, return false
								}
							}
							isCapPtr++;
							wptr++;
						}
					} else
					if (!isupper(*wptr)) {
						// if word is lower, expect all lower except when dict is upper
						while (*wptr != '\0') {
							if (*isCapPtr == -1) {
								// don't need to check cap
								isCapPtr++;
								wptr++;
								continue;
							} else 
							if (*isCapPtr == 0 && isupper(*wptr)) {
								// if dict is lowercase and word is upper, return false
								return 0;
							} else
							if (*isCapPtr == 1 && !isupper(*wptr)) {
								// if dict is uppercase and word is lowercase, return false
								return 0;
							}

							isCapPtr++;
							wptr++;
						}
					}
				}

			}
		} else
		if (*isCapPtr == 1 && !isupper(*wptr)) {
			// if dict is cap and word is lowercase, return false
			return 0;
		}
	}

	return isWord;

}
