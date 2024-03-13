#ifndef DICTIONARY_H
#define DICTIONARY_H

struct dictionary;

// creates a trie out of a given dictionary file and returns the header to a dict struct
public dict makeDict(FILE* file);

// inserts a string into the trie
private void insertString(dict dict, char[] string);

// deletes a string in the trie
private void deleteString(dictionary dict, char[] string);

// searches the dictionary for a word. Returns true if the word is
// in the dictionary, false if the word is not in the dictionary.
public bool isWord(dictionary dict, char[] word);

#endif
