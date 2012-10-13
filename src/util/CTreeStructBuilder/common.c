/* Common code used by other source files */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "header.h"



void printUsage(char *prg){
	printf("Usage:\n");
	printf("%s\n <input file name>\n", prg);
}


char *translateFileName(char *inName, char *append){
	int totalLen = 0;
	int inNameLen = 0;
	int i;
	char *resStr = NULL;
	if (inName == NULL) return NULL;
	
	/* Get number of characters to copy from inName */
	/* First find end of inName. Cut off file extension */
	inNameLen = strlen(inName);
	for (i = inNameLen-1; i > 0; i--) {
		if (inName[i] == '.'){
			/* Cut off here */
			inNameLen = i;
			break;
		} else if (inName[i] == '/'){
			/* No file extension detected */
			break;
		}
	}
	
	/* Get total length of result string */
	if (append == NULL){
		totalLen = inNameLen;
	} else {
		totalLen = inNameLen + strlen(append);
	}
	
	/* Create buffer */
	resStr = (char *) malloc(sizeof(*resStr) * (totalLen + 1));
	if (resStr == NULL) return NULL;
	
	/* Copy input name */
	for (i = 0; i < inNameLen; i++){
		resStr[i] = inName[i];
	}
	
	if (append != NULL){
		/* Copy append string */
		for (; (i < totalLen) && append[i-inNameLen] != '\0'; i++){
			resStr[i] = append[i-inNameLen];
		} 
	}
	
	/* Add final null terminator and return */
	resStr[totalLen] = '\0';
	return resStr;
}




void freeChr(char **ppChar){
	if ( ppChar == NULL) return;
	if (*ppChar == NULL) return;
	
	free(*ppChar);
	
	*ppChar = NULL;
}
