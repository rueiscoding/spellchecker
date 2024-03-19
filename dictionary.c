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
				// if we reach leaf node before processing entire word and current node is a word
				// check if rest of word is just punctuation that can be ignored

				isWord = 1; 

				while ((*wptr) != '\0') {
					// continue to traverse string
				
					if (isalpha(*wptr)) {
						// if there are any letters after valid word, word is invalid	
						return 0;
					}

					*isCapPtr = -1; // don't need to check cap
					isCapPtr++;
					wptr++;
				}
				
				*isCapPtr = -2; // end of word
				break; // break out of loop, end of word
			} else
			if (tptr->isWord == 0) {
				return 0; // word is invalid
			}	

		} 

		if (isalpha(*wptr)) {
			if (isupper(*wptr)) {
				*isCapPtr = 1;
			} else {
				*isCapPtr = 0;
			}
		} else {
			*isCapPtr = -1;
		}

		tptr = tptr->children[index]; // check next character
		wptr++;
		isCapPtr++;
	}

	if ((*tptr).isWord) {
		isWord = 1;
	}

	// check capitalization if spelling is correct
	if (isWord == 1) {
		isCapPtr = isCap; // pointer to capitalization array
		wptr = word;
		
		while (*isCapPtr == -1 && *wptr != '\0') {
			// ignore any invalid characters at beginning
			isCapPtr++;
			wptr++;

		}

		// first valid cap check
		while (*wptr != '\0') {
			if (*isCapPtr == 1) {
				// if dict char is uppercase and word char is lowercase, word is invalid
				if (!isupper(*wptr)) {
					return 0;
				}

				// otherwise both dict and word is uppercase and we can continue 
				wptr++;
				isCapPtr++;
				continue;
			} else
			if (*isCapPtr == 0) {
				if (!isupper(*wptr)) {
					// if dict is lowercase and word is lowercase, pattern is established
					// all characters must be lowercase unless dict has an uppercase
					while (*wptr != '\0') {
						// check pattern for rest of word
						if(*isCapPtr == 0) {
							// if dict is lowercase, word should be lowercase
							if (isupper(*wptr)) {
								return 0; // if word is uppercase, invalid
							}
						} else
						if (*isCapPtr == 1) {
							// if dict is capital, word should also be capital
							if (!isupper(*wptr)) {
								return 0; // if word is lowercase, invalid
							}
						}

						// otherwise, *isCapPtr == -1, so don't check
						// continue
						isCapPtr++;
						wptr++;
					}
				} else
				if (isupper(*wptr)) {
					// if dict is lowercase and word is uppercase, check the cases

					isCapPtr++;
					wptr++;

					while (*wptr != '\0') {
						if (*isCapPtr == -1 || (*isCapPtr == 1 && isupper(*wptr))) {
							// if *isCapPtr is -1, don't check cap
							// if dict is uppercase and word is uppercase, continue
							isCapPtr++;
							wptr++;
						} else
						if (*isCapPtr == 1 && !isupper(*wptr)) {
							// if dict is uppercase and word is lowercase, invalid
							return 0;
						} else 
						if (*isCapPtr == 0 && isupper(*wptr)) {
							// if dict is lowercase and word is uppercase, pattern has been established
							// must all be uppercase
							while (*wptr != '\0') {
								// check that rest of word is uppercase
								if (*isCapPtr != -1) {
									if (!isupper(*wptr))
										return 0; // if character is lowercase, invalid
								}

								// otherwise *isCapPtr is -1, don't check cap
								// continue
								isCapPtr++;
								wptr++;
							}
						} else
						if (*isCapPtr == 0 && !isupper(*wptr)) {
							// if dict is lowercase and word is lowercase, pattern has been established
							// rest must be lowercase unless dict is uppercase
							while (*wptr != '\0') {
								// check that rest of the word is lowercase unless dict is uppercase
								if (*isCapPtr == 0) {
									if (isupper(*wptr)) {
										return 0; // if dict is lower and word is upper, invalid
									}

									// otherwise word is lower and is valid
								} else 
								if (*isCapPtr == 1) {
									if (!isupper(*wptr)) {
										return 0; // if dict is upper and word is lower, invalid
									}

									// otherwise word is upper and is valid
								}

								// otherwise *isCapPtr is -1, don't check cap
								// continue
								wptr++;
								isCapPtr++;
							}
						}
					}
				}
				
			}
		}	
	}

	return isWord;

}
