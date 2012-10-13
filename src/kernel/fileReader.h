/* Used by the kernel to load simple record files into memory as strings */

#ifndef KERNEL_FILE_READER_H
#define KERNEL_FILE_READER_H

/* File loader structures, acts like a linked list string, where
	each string is a chunk of the file.
	The root loader contains the pointer to the first node as well
	as the maximum node buffer size.
	The nodes are a linked list of records, although to cope
	with records larger than the buffer size each record node
	has a linked list of nodes to hold overflow. IOW technically
	a tree structure, although child nodes should be few in
	practice. */
/* Root of file loader list */
struct SLXKr_fldr{
	/* Total capacity of each node's buffer */
	int tSize;
	
	/* First file loader node */
	struct SLXKn_fldr *pChild;
};


/* Node of file loader list */
struct SLXKn_fldr{
	/* Buffer. Not null terminated */
	char *pBuf;
	
	/* Index of last character used in buffer, -1 if empty */
	int i;
	
	/* Next file loader node, representing the next record */
	struct SLXKn_fldr *pNext;
	
	/* Child file loader node, for data of the same record */
	struct SLXKn_fldr *pChild;
};


/* Loads a file into the loader structure. Size is the chunk size
	of bytes that a file is read in at a time. Del is the 
	delimiter separating records */
struct SLXKr_fldr *slxk_loadFile(char *fnm, int size, char del);

/* Returns a newly created filed loader node */
struct SLXKn_fldr *slxk_newFldrNode(int size);

/* Returns a newly created file loader root */
struct SLXKr_fldr *slxk_newFldr(int size);

/* Frees memory associated with a File Loader. If ftl (Free Top Level)
	is non-zero then the root pointer itself will be freed. */
void slxk_freeFldr(struct SLXKr_fldr **ppRoot, int ftl);

/* Recursively frees the child and sybling nodes of a given node,
	then frees itself if ftl is true */
void slxk_freeFldrNode(struct SLXKn_fldr **ppNode, int ftl);

/* Recursively concatenates node's and children's
	buffers, then returns as c string. First call for the
	recursive function. Calls slxk_fldrRecToStrRec to
	handle actual concats. First runs through the list adding
	up the total string length, then goes up the call trace
	adding the buffers to the final string */
char *slxk_fldrRecToStr(struct SLXKn_fldr *pNode);
char *slxk_fldrRecToStrRec(struct SLXKn_fldr *pNode, int *tLen, int *i);


#endif
