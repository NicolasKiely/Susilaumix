#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "loader.h"
#include "header.h"



struct token *loadInputFile(char *fileName){
	int c = 0; /* Character input from file */
	int nest = 0; /* Flag for nested in brackets */
	int charCount = 0; /* Number of characters read for latest token */
	

	/* Attempt to open input file */
	FILE *fh = fopen(fileName, "r");
	if (fh == NULL){
		printf("Error, could not open file '%s' for reading\n", fileName);
		return NULL;
	}
	
	/* Initialize list of tokens */
	struct token *pToken = initToken();
	struct token *pCurrent = pToken;
	if (pToken == NULL){
		printf("Error, could not allocate memory for tokens\n");
		return NULL;
	}
	
	/* Read characters */
	while ( (c = fgetc(fh)) != EOF ){
		if (c == '[') {
			/* Nest into brackets */
			nest = -1;
			
		} else if (c == ']') {
			/* Break out of brackets */
			nest = 0;
			
		} else if (isspace(c) && (nest == 0)) {
			/* Whitespace encountered, and not nested, so attempt delimit */
			
			if (pCurrent->pText == NULL) {
				/* Last node is empty, so don't do anything */
				continue;
				
			} else {
				/* Clean up old node: terminate with null */
				if (charCount < bufferSize) {
					pCurrent->pText[charCount] = '\0';
				} else {
					pCurrent->pText[bufferSize] = '\0';
				}
				
				/* Create new node */
				struct token *pNew = initToken();
				if (pNew == NULL) {
					printf("Error, could not create token\n");
					freeToken(&pToken);
					return NULL;
				}
				
				/* Point to new node */
				pCurrent->pNext = pNew;
				pCurrent = pNew;
			}
				
		} else {
			/* Add character */
			if (pCurrent->pText == NULL) {
				/* Initialize buffer first, and reset counter */
				pCurrent->pText = malloc(bufferSize);
				charCount = 0;
				
				if (pCurrent->pText == NULL) {
					printf("Error, could not allocate token text memory\n");
					freeToken(&pToken);
					return NULL;
				}
			}
			
			/* Display warning in case input token is too long */
			if (charCount+1 == bufferSize) printf("Warning: buffer overflow, truncating line\n");
			
			/* Set character in token */
			if (charCount+1 < bufferSize) pCurrent->pText[charCount] = c;
			
			/* Increment character count */
			charCount++;
		}
	}
	
	/* Clean up old node in case it hasn't been null-terminated */
	if (pCurrent->pText != NULL){
		if (charCount < bufferSize){
			pCurrent->pText[charCount] = '\0';
		} else {
			pCurrent->pText[bufferSize-1] = '\0';
		}
	}
	
	/* Free resources */
	fclose(fh);
	
	/* Done */
	return pToken;
}


struct token *initToken(){
	struct token *pToken = malloc(sizeof(*pToken));
	if (pToken == NULL) return NULL;
	
	pToken->pText = NULL;
	pToken->pNext = NULL;
	
	return pToken;
}


void freeToken(struct token **ppToken){
	if ( ppToken == NULL) return;
	if (*ppToken == NULL) return;
	
	freeChr(  &( (*ppToken)->pText ));
	freeToken(&( (*ppToken)->pNext ));
	
	free(*ppToken);
	
	*ppToken = NULL;
}
