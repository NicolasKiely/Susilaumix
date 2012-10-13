#include <stdlib.h>

#include "structure.h"
#include "header.h"


struct str *initStr(){
	struct str *pStr = malloc(sizeof(*pStr));
	if (pStr == NULL) return NULL;
	
	pStr->pName = NULL;
	pStr->pVar  = NULL;
	pStr->pBuf  = NULL;
	pStr->pPtr  = NULL;
	pStr->pNext = NULL;
	
	return pStr;
}


struct var *initVar(){
	struct var *pVar = malloc(sizeof(*pVar));
	if (pVar == NULL) return NULL;
	
	pVar->pType = NULL;
	pVar->pName = NULL;
	pVar->pVal  = NULL;
	pVar->pNext = NULL;
	
	return pVar;
}


struct buf *initBuf(){
	struct buf *pBuf = malloc(sizeof(*pBuf));
	if (pBuf == NULL) return NULL;
	
	pBuf->pType = NULL;
	pBuf->pName = NULL;
	pBuf->pSize = NULL;
	pBuf->pVal  = NULL;
	pBuf->pNext = NULL;
	
	return pBuf;
}


struct ptr *initPtr(){
	struct ptr *pPtr = malloc(sizeof(*pPtr));
	if (pPtr == NULL) return NULL;
	
	pPtr->pType = NULL;
	pPtr->pName = NULL;
	pPtr->pNext = NULL;
	pPtr->doInit = 0;
	
	return pPtr;
}


void freeStr(struct str **ppStr){
	if ( ppStr == NULL) return;
	if (*ppStr == NULL) return;
	
	freeChr(&( (*ppStr)->pName ));
	freeVar(&( (*ppStr)->pVar  ));
	freeBuf(&( (*ppStr)->pBuf  ));
	freePtr(&( (*ppStr)->pPtr  ));
	freeStr(&( (*ppStr)->pNext ));
	
	free(*ppStr);
	
	*ppStr = NULL;
}


void freeVar(struct var **ppVar){
	if ( ppVar == NULL) return;
	if (*ppVar == NULL) return;
	
	freeChr(&( (*ppVar)->pType ));
	freeChr(&( (*ppVar)->pName ));
	freeChr(&( (*ppVar)->pVal  ));
	freeVar(&( (*ppVar)->pNext ));
	
	free(*ppVar);
	
	*ppVar = NULL;
}


void freeBuf(struct buf **ppBuf){
	if ( ppBuf == NULL) return;
	if (*ppBuf == NULL) return;
	
	freeChr(&( (*ppBuf)->pType ));
	freeChr(&( (*ppBuf)->pName ));
	freeChr(&( (*ppBuf)->pSize ));
	freeChr(&( (*ppBuf)->pVal  ));
	freeBuf(&( (*ppBuf)->pNext ));
	
	free(*ppBuf);
	
	*ppBuf = NULL;
}


void freePtr(struct ptr **ppPtr){
	if ( ppPtr == NULL) return;
	if (*ppPtr == NULL) return;
	
	freeChr(&( (*ppPtr)->pType ));
	freeChr(&( (*ppPtr)->pName ));
	freePtr(&( (*ppPtr)->pNext ));
	
	free(*ppPtr);
	
	*ppPtr = NULL;
}
