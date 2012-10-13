#include <stdio.h>
#include <stdlib.h>

#include "fileReader.h"


struct SLXKr_fldr *slxk_loadFile(char *fnm, int size, char del){
	FILE              *pFhdl = NULL; /* File handle */
	struct SLXKr_fldr *pFldr = NULL; /* File loader list */
	int                c     = '\0'; /* Input character */
	/* Following two are used to iterated node pointers */
	struct SLXKn_fldr *pRec  = NULL; /* First node for current record */
	struct SLXKn_fldr *pCur  = NULL; /* Current node */

	/* Attempt to open file for reading */
	pFhdl = fopen(fnm, "r");
	if (pFhdl == NULL){
		return NULL;
	}
	
	/* Initialize the loader structure */
	pFldr = slxk_newFldr(size);
	if (pFldr == NULL){
		fclose(pFhdl);
		return NULL;
	}
	
	/* Initialize first node */
	pFldr->pChild = slxk_newFldrNode(pFldr->tSize);
	if (pFldr->pChild == NULL) {
		slxk_freeFldr(&pFldr, -1);
		fclose(pFhdl);
		return NULL;
	}
	
	/* Loop through each character in the file */
	pRec = pFldr->pChild;
	pCur = pRec;
	while ( (c = fgetc(pFhdl)) != EOF){
		if (c == del) {
			/* Delimiter found, try to move to next record */
			int d = fgetc(pFhdl);
			if (d == EOF) {
				/* Never mind, end of file. Bail out */
				break;
			} else {
				/* Put character back */
				ungetc(d, pFhdl);
			}
			
			/* Create new node for the next record, and point to it */
			pRec->pNext = slxk_newFldrNode(pFldr->tSize);
			pRec = pRec->pNext;
			pCur = pRec;
			
			if (pRec == NULL) {
				/* Allocation Error */
				slxk_freeFldr(&pFldr, -1);
				fclose(pFhdl);
				return NULL;
			}
		} else {
			/* Add character to current record */
			if (pCur->i >= pFldr->tSize) {
				/* Current node full; create and use child */
				pCur->pChild = slxk_newFldrNode(pFldr->tSize);
				pCur = pCur->pChild;
				if (pCur == NULL) {
					/* Allocation Error */
					slxk_freeFldr(&pFldr, -1);
					fclose(pFhdl);
					return NULL;
				}
			}
			
			/* Increment index and set the character */
			pCur->i++;
			pCur->pBuf[pCur->i] = c;
		}
	}
	
	fclose(pFhdl);
	return pFldr;
}


struct SLXKn_fldr *slxk_newFldrNode(int size){
	struct SLXKn_fldr *pNewFldr = NULL;
	
	pNewFldr = (struct SLXKn_fldr *) malloc(sizeof(*pNewFldr));
	if (pNewFldr == NULL)
		return NULL;
		
	pNewFldr->pBuf = (char *) malloc(sizeof(*(pNewFldr->pBuf)) * size);
	if (pNewFldr->pBuf == NULL) {
		free(pNewFldr);
		return NULL;
	}
	pNewFldr->i = -1;
	pNewFldr->pNext = NULL;
	pNewFldr->pChild = NULL;
	
	return pNewFldr;
}


void slxk_freeFldr(struct SLXKr_fldr **ppRoot, int ftl){
	struct SLXKr_fldr *pRoot = NULL;
	
	if (ppRoot == NULL)
		return;
	
	/* Step down the pointer */
	pRoot = *ppRoot;
	
	if (pRoot == NULL)
		return;
	
	/* Recursively free nodes */
	slxk_freeFldrNode(& (pRoot->pChild), -1);
	
	if (ftl != 0) {
		/* Free the root itself and set original pointer to null */
		free(pRoot);
		*ppRoot = NULL;
	}
}


void slxk_freeFldrNode(struct SLXKn_fldr **ppNode, int ftl){
	struct SLXKn_fldr *pNode = NULL;
	
	if (ppNode == NULL)
		return;
	
	/* Step pointer down */
	pNode = *ppNode;
	if (pNode == NULL)
		return;
	
	/* Free child and sybling nodes */
	slxk_freeFldrNode(& (pNode->pChild), -1);
	slxk_freeFldrNode(& (pNode->pNext), -1);
	
	/* Free buffer */
	if (pNode->pBuf != NULL)
		free(pNode->pBuf);
	
	/* Free self and set original pointer to null */
	if (ftl){
		free(pNode);
		*ppNode = NULL;
	}
}


struct SLXKr_fldr *slxk_newFldr(int size){
	struct SLXKr_fldr *pFldr = NULL;
	
	pFldr = (struct SLXKr_fldr *) malloc(sizeof(*pFldr));
	
	if (pFldr == NULL)
		return NULL;
	
	pFldr->tSize  = size;
	pFldr->pChild = NULL;
	
	return pFldr;
}


char *slxk_fldrRecToStr(struct SLXKn_fldr *pNode){
	int tLen = 0;
	int i    = 0;
	return slxk_fldrRecToStrRec(pNode, &tLen, &i);
}


char *slxk_fldrRecToStrRec(struct SLXKn_fldr *pNode, int *tLen, int *i){
	char *pStr = NULL;
	int j;
	
	if (pNode == NULL || tLen == NULL || i == NULL)
		return NULL;
	
	/* Add this buffer's length to total string length */
	*tLen += pNode->i + 1;
	
	if (pNode->pChild == NULL) {
		/* End of node list, allocate memory for final string */
		pStr = (char *) malloc(sizeof(*pStr) * (*tLen + 1));
		if (pStr == NULL)
			return NULL;
		
		/* Set last character to null char */
		pStr[*tLen] = '\0';
		
		/* Write buffer to end of string */
		if (pNode->pBuf != NULL){
			for (j = 0; j <= pNode->i; j++){
				pStr[*tLen - (pNode->i+1) + j] = pNode->pBuf[j];
			}
			/* Set index for parent node, which is the string length from buffer end */
			*i = *tLen - (pNode->i+1);
		}
		
		/* Return for parent to use */
		return pStr;
		
	} else {
		/* Not at end of node list, let children do their work first */
		pStr = slxk_fldrRecToStrRec(pNode->pChild, tLen, i);
		if (pStr == NULL)
			return NULL;
		
		/* Write buffer to section of string before child's section */
		for (j = 0; j <= pNode->i; j++) {
			pStr[*i - (pNode->i+1) + j] = pNode->pBuf[j];
		}
		
		/* Set index for parent node, which is string length from previous index */
		*i -= (pNode->i+1);
		
		/* Return for parent to use */
		return pStr;
	}
}
