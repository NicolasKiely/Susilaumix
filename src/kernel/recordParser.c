#include <stdlib.h>
#include <ctype.h>

#include "recordParser.h"


struct SLXKr_prpt *slxk_parseFldrRecs(struct SLXKr_fldr *pFldr, char sep){
	struct SLXKn_fldr *pFldrNode; /* Used in loop to copy from fileloader nodes */
	struct SLXKn_prpt *pPrptNode; /* Used in loop to copy to property nodes */
	struct SLXKr_prpt *pPrpt;     /* New property list to generate and return */
	
	if (pFldr == NULL)
		return NULL;
	
	/* Initialize the property list */
	pPrpt = slxk_newPrpt();
	if (pPrpt == NULL)
		return NULL;
		
	/* Load first node */
	pPrpt->pChild = slxk_parseRecord(slxk_fldrRecToStr(pFldr->pChild), sep);
	if (pPrpt->pChild == NULL){
		slxk_freePrpt(&pPrpt, -1);
		return NULL;
	}
	
	/* Loop through the rest of the file loader nodes */
	pFldrNode = pFldr->pChild->pNext;
	pPrptNode = pPrpt->pChild;
	while (pFldrNode != NULL) {
		/* Parse next record */
		pPrptNode->pNext = slxk_parseRecord(slxk_fldrRecToStr(pFldrNode), sep);
		if (pPrptNode->pNext == NULL) {
			/* Failed to get record parsed */
			//slxk_freePrpt(&pPrpt, -1);
			//return NULL;
		} else {
			pPrptNode = pPrptNode->pNext;
		}
		
		/* Iterate */
		pFldrNode = pFldrNode->pNext;
	}
	
	return pPrpt;
}


struct SLXKn_prpt *slxk_parseRecord(char *pBuf, char sep){
	int iSep;                 /* Index of the key-value separator */
	struct SLXKn_prpt *pPrpt; /* New property */
	
	/* Allocate memory for property structure */
	pPrpt = slxk_newPrptNode();
	if (pPrpt == NULL)
		return NULL;
	
	/* Look for separator */
	iSep = slxk_findRecSep(pBuf, sep);
	
	if (iSep == -1){
		slxk_freePrptNode(&pPrpt, -1);
		return NULL;
	}
	
	/* Grab key */
	pPrpt->pKey = slxk_getRecKey(pBuf, iSep);
	if (pPrpt->pKey == NULL){
		slxk_freePrptNode(&pPrpt, -1);
		return NULL;
	}
	
	/* Grab value */
	pPrpt->pVal = slxk_getRecVal(pBuf, iSep);
	if (pPrpt->pVal == NULL){
		slxk_freePrptNode(&pPrpt, -1);
		return NULL;
	}
	
	return pPrpt;
}


int slxk_findRecSep(char *pBuf, char sep){
	int i;
	
	if (pBuf == NULL)
		return -1;
	
	/* Loop through characters of the buffer */
	for (i = 0; pBuf[i] != '\0'; i++){
		/* Check to see if character matches separator */
		if (pBuf[i] == sep)
			return i;
	}
	
	/* No match found */
	return -1;
}


char *slxk_getRecKey(char *pBuf, int iSep){
	int iBeg;   /* Index of begining of key string */
	int iEnd;   /* Index of end of key string */
	int i;
	int num;    /* Number of characters to copy */
	char *pKey; /* Key buffer */
	
	if (pBuf == NULL || iSep == -1)
		return NULL;
	
	/* Find position to start copying */
	for (iBeg = 0; ; iBeg++){
		/* Continue incrementing start position if whitespace */
		if (isspace(pBuf[iBeg]))
			continue;
		
		/* Fail if end of string or hit separator */	
		if (pBuf[iBeg] == '\0')
			return NULL;
		if (iBeg >= iSep)
			return NULL;
		
		/* Found first "normal" character, use this to start */
		break;
	}
	
	/* Find position of end of key */
	for (iEnd = iSep-1; ; iEnd--){
		/* Fail if past begining of string. Second case shouldn't happen */
		if (iEnd < 0)
			return NULL;
		if (iEnd < iBeg)
			return NULL;
		
		/* Ignore whitespace */
		if (isspace(pBuf[iEnd]))
			continue;
		
		/* Found last "normal" character, use this as end */
		break;
	}
	
	/* Find number of characters to copy */
	num = iEnd - iBeg + 1;
	
	/* Allocate memory for key buffer */
	pKey = (char *) malloc(sizeof(*pKey) * (num +1 ));
	if (pKey == NULL)
		return NULL;
	
	/* Copy characters from record to key buffer */
	for (i = iBeg; i <= iEnd; i++)
		pKey[i - iBeg] = pBuf[i];
	
	/* Add null terminator */
	pKey[num] = '\0';
	
	/* Return buffer */
	return pKey;
}


char *slxk_getRecVal(char *pBuf, int iSep){
	int iBeg;   /* Index of beginning of value string */
	int iEnd;   /* Index of end of value string */
	int i;
	int num;    /* Number of characters to copy */
	char *pVal; /* Buffer to handle value string */
	
	if (pBuf == NULL || iSep < 0)
		return NULL;
	
	/* Find position to start copying */
	for (iBeg = iSep + 1; ; iBeg++){
		/* Fail if end of string and no content found */
		if (pBuf[iBeg] == '\0')
			return NULL;
		
		/* Skip white space */
		if (isspace(pBuf[iBeg]))
			continue;
			
		/* First normal character found, start here */
		break;
	}
	
	/* Find position to end copying */
	iEnd = iBeg;
	for (i = iBeg; pBuf[i] != '\0'; i++){
		/* Skip if whitespace */
		if (isspace(pBuf[i]))
			continue;
		
		/* Not whitespace and not the end, so set end index to here */
		iEnd = i;
	}
	
	/* Find number of characters to copy */
	num = iEnd - iBeg + 1;
	
	/* Allocate memory for value buffer */
	pVal = (char *) malloc(sizeof(*pVal)*(num + 1));
	if (pVal == NULL)
		return NULL;
		
	/* Copy over value string to the new buffer */
	for (i = iBeg; i <= iEnd; i++)
		pVal[i-iBeg] = pBuf[i];
	
	/* Add null terminator */
	pVal[num] = '\0';
	
	return pVal;
}


struct SLXKr_prpt *slxk_newPrpt(){
	struct SLXKr_prpt *pPrpt;
	
	pPrpt = (struct SLXKr_prpt *) malloc(sizeof(*pPrpt));
	if (pPrpt == NULL)
		return NULL;
	
	pPrpt->pChild = NULL;
	
	return pPrpt;
}


void slxk_freePrpt(struct SLXKr_prpt **ppPrpt, int ftl){
	struct SLXKr_prpt *pPrpt;
	
	if (ppPrpt == NULL)
		return;
	
	/* Step pointer down */
	pPrpt = *ppPrpt;
	if (pPrpt == NULL)
		return;
	
	/* Free child nodes */
	if (pPrpt->pChild != NULL)
		slxk_freePrptNode(& (pPrpt->pChild), -1);
	
	/* Free self if specified */
	if (ftl){
		free(pPrpt);
		*ppPrpt = NULL;
	}
}


struct SLXKn_prpt *slxk_newPrptNode(){
	struct SLXKn_prpt *pPrpt;
	
	pPrpt = (struct SLXKn_prpt *) malloc(sizeof(*pPrpt));
	if (pPrpt == NULL)
		return NULL;
	
	pPrpt->pKey  = NULL;
	pPrpt->pVal  = NULL;
	pPrpt->pNext = NULL;
	
	return pPrpt;
}


void slxk_freePrptNode(struct SLXKn_prpt **ppPrpt, int ftl){
	struct SLXKn_prpt *pPrpt;
	
	if (ppPrpt == NULL)
		return;
	
	/* Step down pointer */
	pPrpt = *ppPrpt;
	if (pPrpt == NULL)
		return;
		
	/* Free children */
	slxk_freePrptNode(& (pPrpt->pNext), -1);
	
	/* Free buffers */
	if (pPrpt->pKey != NULL)
		free(pPrpt->pKey);
	if (pPrpt->pVal != NULL)
		free(pPrpt->pVal);
	
	if (ftl){
		/* Free self */
		free(pPrpt);
	
		/* Set original pointer to null */
		*ppPrpt = NULL;
	}
}
