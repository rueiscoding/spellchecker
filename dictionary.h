#ifndef DICTIONARY_H
#define DICTIONARY_H

struct Node;

/* param file: the name of the dictionary text file
 * returns: pointer to the head of the dictionary trie
 * summary:  creates a trie out of a given dictionary file and returns the pointer to the head of the trie
 */
struct Node* makeDict(char filename[]);

/* param c: the character to convert to index
 * returns: int that represents index associated with given character
 */
int hash(char c); 

/* param dict: pointer the head of the trie
 * summary: frees the trie
 */
void freeDict(struct Node* head);

/* param dict: pointer to the head of the trie
 * param word: the word to verify
 * returns: 1 if the word is valid, 0 if the word is invalid
 * summary: searches the dictionary for a word. Returns 1 if the word is in the dictionary, 0 if the word is not in the dictionary.
 */
int isWord(struct Node* head, char word[]);

#endif
