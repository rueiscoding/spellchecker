# spellchecker

Authors: Regine Delacruz and Arunima Tripathy
netids: rld140, at1206

Our implementation of spellchecker is as follows:
(1) dictionary.c: This program converts a dictionary file into a trie data structure. Additionally, it verifies the validity of words by traversing the trie.
(2) traverse.c: A program designed to recursively navigate through directories.
(3) readfile.c: This program reads input files and identifies misspelled words within the file, marking them with row and column numbers.
(4) driver.c: This program integrates all parts of the spellchecker together.

More on dictionary implementation:
We decided on a trie structure that utilizes a psuedo hash mapping to access children of a node. Each node has an array of children, with each element representing a different possible character. At first, we considered a linked list for the children, but we ultimately decided on an array for a lookup complexity of O(1). Using a simple hash function, we convert a given character into an index to quickly find the associated node while traversing the trie. 

Features:
(1) Quotation marks '"' '\'' and brackets '(' ')' '[' ']' '{' '}' are ignored at the start of a word. Angled brackets are considered as the writeup does not specify angled brackets should be ignored.
(2) Words with hyphens are valid only if all components are valid.
(3) Variations on capitalization, including exact capitalization, intial capitalization, all capitalization, and all lowercase (with the exception of letters that are capitalized in the dictionary) are valid so long as the spelling is valid and any capitalization in a dictionary
word is matched.
(4) All directories are traversed and all files are processed.
(5) Errors are printed according to writeup.

Test cases:
We have a test directory ("test") and test dictionary ("testdict.txt") that, when run as arguments to our program, tests all the features above. For testing the reading of directories, test has multiple subdirectories, and those subdirectories also contain their own subdirectories. We included an empty directory to ensure that the program considers that case, and included an empty text file as well.

emptydir: an empty directory

test directory:
empty.txt - an empty text file
test0.txt - regular words containing only alphabetical characters
test1.txt - words containing only numbers
test2.txt - words containing alphanumeric characters

level2 directory:
test3.txt - valid words starting with ignored characters
test4.txt - valid words ending with ignored characters

lmore directory:
test1.txt - words starting and ending with ignored characters

lmore2 directory:
test1.txt - more tests, words with punctuation between

level3 directory:
hyphens.txt - tests hyphenated words, including words that end with ignored punctuation and a hyphen followed by ignored punctuation
capitals.txt - tests capitals

godtier directory:
thebeemovie.txt - the bee movie transcript
