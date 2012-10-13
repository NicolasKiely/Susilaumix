/* The purpose of this utility is to generate scaffold *.c and *.h files
	for c tree structures.
	
	Input file format:
	structure <structure name>
		variable <variable type> <variable name> [initial value]
		variable <variable type> <variable name> [initial value]
		...
		buffer <variable type> <variable name> [memory to allocate] [initial value]
		buffer <variable type> <variable name> [memory to allocate] [initial value]
		...
		pointer <structure type> <variable name> [INIT | NO_INIT]
		pointer <structure type> <variable name> [INIT | NO_INIT]
		...
	end
	
	structure <structure name>
		...
	end
	...
	
	Things to put in output file:
	Structure definitions
	Structure initialization
	Structure freeing
*/

#include <stdio.h>
#include <stdlib.h>

#include "header.h"
#include "loader.h"
#include "structure.h"


int main(int argc, char *argv[]){
	char *inFile = NULL;
	struct token *pToken = NULL;
	struct str *pStr = NULL;
	
	/* Display help if incorrect arguments */
	if (argc != 2) {
		printUsage(argv[0]);
		return 1;
	}
	
	/* Get input file's name */
	inFile = argv[1];
	
	/* Attempt to load file */
	pToken = loadInputFile(inFile);
	if (pToken == NULL){
		printf("--Load failure\n");
		return 1;
	}
	
	/* Debug token list */
	struct token *pCurrent;
	for (pCurrent = pToken; pCurrent != NULL; pCurrent = pCurrent->pNext){
		printf("'%s'\n", pCurrent->pText);
	}
	
	
	/* Free resources */
	freeToken(&pToken);
	freeStr(&pStr);
	
	/* Done */
	return 0;
}
