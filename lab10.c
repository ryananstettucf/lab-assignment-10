#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{	
    struct TrieNode *root;
};

struct TrieNode {
    struct TrieNode *children[26];
    int count;                     
};

struct TrieNode* createNode(void) {
    struct TrieNode *newNode = malloc(sizeof(struct TrieNode));
    if (newNode) {
        newNode->count = 0;
        for (int i = 0; i < 26; i++) {
            newNode->children[i] = NULL;
        }
    }
    return newNode;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
    int level;
    int length = strlen(word);
    int index;

    struct TrieNode *pCrawl = pTrie->root;

    for (level = 0; level < length; level++) {
        index = word[level] - 'a';
        if (!pCrawl->children[index]) {
            pCrawl->children[index] = createNode();
        }
        pCrawl = pCrawl->children[index];
    }

    pCrawl->count += 1;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
    int level;
    int length = strlen(word);
    int index;
    struct TrieNode *pCrawl = pTrie->root;

    for (level = 0; level < length; level++) {
        index = word[level] - 'a';
        if (!pCrawl->children[index]) {
            return 0;
        }
        pCrawl = pCrawl->children[index];
    }

    return pCrawl->count;
}

// Deallocates the trie nodes recursively
void deallocateTrieNode(struct TrieNode *node) {
    if (node != NULL) {
        for (int i = 0; i < 26; i++) {
            if (node->children[i] != NULL) {
                deallocateTrieNode(node->children[i]);
            }
        }
        free(node);
    }
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
    if (pTrie != NULL) {
        deallocateTrieNode(pTrie->root);
        free(pTrie);
        return NULL;
    }
    return pTrie;
}

// Initializes a trie structure
struct Trie *createTrie()
{
    struct Trie *trie = (struct Trie *)malloc(sizeof(struct Trie));
    if (trie) {
        trie->root = createNode();
    }
    return trie;
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Could not open file %s\n", filename);
        return 0;
    }

    int wordCount, expectedCount;
    if (fscanf(file, "%d", &expectedCount) != 1) { 
        printf("Failed to read the number of words.\n");
        fclose(file);
        return 0;
    }

    char buffer[100];
    wordCount = 0;

    while (wordCount < expectedCount && fscanf(file, "%99s", buffer) == 1) { 
        pInWords[wordCount] = strdup(buffer);
        wordCount++;
    }

    fclose(file);
    return wordCount;
}


int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}