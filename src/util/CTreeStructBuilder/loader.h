/* Manages loading input file into representative data structures */

#ifndef LOADER_H
#define LOADER_H


/* Linked list of tokens */
struct token{
	/* Text of the token */
	char *pText;
	
	/* Next token in the list */
	struct token *pNext;
};


/* Loads input file into list of tokens */
struct token *loadInputFile(char *fileName);



/* Initialize/Free token */
struct token *initToken();
void freeToken(struct token **ppToken);


#endif
