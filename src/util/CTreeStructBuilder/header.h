/* Header for common/main code */
#ifndef C_TREE_STRUCT_BUILDER_HEADER_H
#define C_TREE_STRUCT_BUILDER_HEADER_H

/* Max size of a buffer used in this program, this limit
	shouldn't cause an issue for reasonable input files.
 */
#define bufferSize (250)

/* Prints program usage to screen */
void printUsage(char *prg);

/* Generates an output file name from an input file name,
	with the file extension appended */
char *translateFileName(char *inName, char *append);

/* Used to free c strings */
void freeChr(char **ppChar);


#endif
