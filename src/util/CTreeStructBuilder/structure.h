/* Represents data structures */
#ifndef STRUCTURE_H
#define STRUCTURE_H

/* Linked list of structures */
struct str {
	/* String representing name */
	char *pName;

	/* List of variables */
	struct var *pVar;
	
	/* List of buffers */
	struct buf *pBuf;
	
	/* List of pointers */
	struct ptr *pPtr;
	
	/* Next structure */
	struct str *pNext;
};


/* Linked list of variables */
struct var {
	/* String representing type */
	char *pType;
	
	/* String representing name */
	char *pName;
	
	/* String representing initial value */
	char *pVal;
	
	/* Next variable in list */
	struct var *pNext;
};


/* Linked list of buffers */
struct buf{
	/* String representing type */
	char *pType;
	
	/* String representing name */
	char *pName;
	
	/* String representing size */
	char *pSize;
	
	/* String representing initial value */
	char *pVal;
	
	/* Next buffer in list */
	struct buf *pNext;
};


/* Linked list of pointers */
struct ptr{
	/* String representing referenced structure */ 
	char *pType;
	
	/* String representing name */
	char *pName;
	
	/* Next pointer in list */
	struct ptr *pNext;
	
	/* Flag indicating whether or not to initialize pointer*/
	int doInit;
};


/* Initializers */
struct str *initStr();
struct var *initVar();
struct buf *initBuf();
struct ptr *initPtr();


/* Recursive deallocators */
void freeStr(struct str **ppStr);
void freeVar(struct var **ppVar);
void freeBuf(struct buf **ppBuf);
void freePtr(struct ptr **ppPtr);


#endif
