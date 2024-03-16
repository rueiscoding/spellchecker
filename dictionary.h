#ifndef DICTIONARY_H
#define DICTIONARY_H

struct dictionary;

/* param file: the name of the dictionary text file
 * returns: pointer to the head of the dictionary trie
 * summary:  creates a trie out of a given dictionary file and returns the pointer to the head of the trie
 */
dictionary* makeDict(char[] filename);

/* param dict: pointer the head of the trie
 * summary: frees the trie
 */
void freeDict(dictionary* dict);

/* param dict: pointer the head of the trie
 * param word: the word to add to the trie
 * summary: inserts a word into the trie
 */
void insertWord(dictionary* dict, char[] word);

/* param dict: pointer the head of the trie
 * param word: the word to delete from the trie
 * summary: deletes a word in the trie
 */
void deleteWord(dictionary* dict, char[] word);

/* param dict: pointer to the head of the trie
 * param word: the word to verify
 * returns: true if the word is valid, false if the word is invalid
 * summary: searches the dictionary for a word. Returns true if the word is in the dictionary, false if the word is not in the dictionary.
 */
bool isWord(dictionary* dict, char[] word);

#endif
