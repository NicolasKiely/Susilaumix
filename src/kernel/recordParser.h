/* Used by the kernel to parse basic key-value files loaded with the file reader */

#ifndef KERNEL_RECORD_PARSER_H
#define KERNEL_RECORD_PARSER_H

#include "fileReader.h"


/* Property structures, represent key-value records parsed from the file loaders */
/* Root of a property list */
struct SLXKr_prpt {
	struct SLXKn_prpt *pChild;
};

/* Node of a property list */
struct SLXKn_prpt {
	/* Key string */
	char *pKey;
	
	/* Value string */
	char *pVal;
	
	/* Next property */
	struct SLXKn_prpt *pNext;
};

/* Parse fileloader structure and return full property list */
struct SLXKr_prpt *slxk_parseFldrRecs(struct SLXKr_fldr *pFldr, char sep);

/* Parses a record and returns as a new property node */
struct SLXKn_prpt *slxk_parseRecord(char *pBuf, char sep);

/* Locates the key-value separator in a buffer, -1 if nonexeistant */
int slxk_findRecSep(char *pBuf, char sep);

/* Returns a newly created key string from a record */
char *slxk_getRecKey(char *pBuf, int iSep);

/* Returns a newly created value string from a record */
char *slxk_getRecVal(char *pBuf, int iSep);

/* Creates a newly allocated property root structure */
struct SLXKr_prpt *slxk_newPrpt();

/* Frees a property list, including the root structure if ftl is true */
void slxk_freePrpt(struct SLXKr_prpt **ppPrpt, int ftl);

/* Creates a newly allocated property node */
struct SLXKn_prpt *slxk_newPrptNode();

/* Recursively frees a property node's children, including the structure itself if ftl is true */
void slxk_freePrptNode(struct SLXKn_prpt **ppPrpt, int ftl);

#endif
