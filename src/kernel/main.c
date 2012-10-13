#include <string.h>
#include <stdio.h>

#include "fileReader.h"
#include "recordParser.h"


int main(int argc, char *argv[]){
	int   i   = 0;  /* For loops */
	char *pCfg = NULL;  /* Config file name */
	
	/* Read command line arguments */
	for (i = 1; i < argc; i++){
		if (strcasecmp(argv[i], "--config_file") == 0){
			/* Setting config file, check next argument for file name */
			i++;
			if (i < argc){
				pCfg = argv[i];
				
			} else {
				printf("Error, using '--config_file' with no file\n");
			}
		}
	}
	
	/* Read config file */
	if (pCfg == NULL) {
		pCfg = "config.txt";
	}
	printf("Using config file '%s'\n", pCfg);
	struct SLXKr_fldr *pFldr = slxk_loadFile(pCfg, 16, '\n');
	if (pFldr == NULL){
		printf("Error in loading config\n");
		return 0;
	}
	
	/* Parse file */
	struct SLXKr_prpt *pPrpt = slxk_parseFldrRecs(pFldr, '=');
	if (pPrpt == NULL) {
		printf("Error in parsing config\n");
	}
	
	/* Free reader/parser structs */
	slxk_freeFldr(&pFldr, -1);
	slxk_freePrpt(&pPrpt, -1);
	
	
	/* Exit */
	return 0;
}
