#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "loader.h"
#include "structure.h"


struct FSM_state *initFSM(){
	struct FSM_state *pArray;
	
	/* Attempt to create array of FSM_states */
	pArray = malloc(sizeof(*pArray) * (1+st_LAST));
	if (pArray == NULL) {
		printf("Error, could not allocate array of states\n");
		return NULL;
	}
	
	/* Assign evaluation functions */
	pArray[st_NONE          ].evaluate = &es_NONE;
	pArray[st_ERROR         ].evaluate = &es_ERROR;
	pArray[st_STRUCTURE     ].evaluate = &es_STRUCTURE;
	pArray[st_STRUCTURE_NAME].evaluate = &es_STRUCTURE_NAME;
	pArray[st_STRUCTURE_END ].evaluate = &es_STRUCTURE_END;
	pArray[st_VARIABLE      ].evaluate = &es_VARIABLE;
	pArray[st_VARIABLE_TYPE ].evaluate = &es_VARIABLE_TYPE;
	pArray[st_VARIABLE_NAME ].evaluate = &es_VARIABLE_NAME;
	pArray[st_VARIABLE_INIT ].evaluate = &es_VARIABLE_INIT;
	pArray[st_BUFFER        ].evaluate = &es_BUFFER;
	pArray[st_BUFFER_TYPE   ].evaluate = &es_BUFFER_TYPE;
	pArray[st_BUFFER_NAME   ].evaluate = &es_BUFFER_NAME;
	pArray[st_BUFFER_SIZE   ].evaluate = &es_BUFFER_SIZE;
	pArray[st_BUFFER_INIT   ].evaluate = &es_BUFFER_INIT;
	pArray[st_POINTER       ].evaluate = &es_POINTER;
	pArray[st_POINTER_TYPE  ].evaluate = &es_POINTER_TYPE;
	pArray[st_POINTER_NAME  ].evaluate = &es_POINTER_NAME;
	pArray[st_POINTER_INIT  ].evaluate = &es_POINTER_INIT;
	pArray[st_LAST          ].evaluate = &es_LAST;
	
	/* Assign nextState functions */
	pArray[st_NONE          ].nextState = &ns_NONE;
	pArray[st_ERROR         ].nextState = &ns_ERROR;
	pArray[st_STRUCTURE     ].nextState = &ns_STRUCTURE;
	pArray[st_STRUCTURE_NAME].nextState = &ns_STRUCTURE_NAME;
	pArray[st_STRUCTURE_END ].nextState = &ns_STRUCTURE_END;
	pArray[st_VARIABLE      ].nextState = &ns_VARIABLE;
	pArray[st_VARIABLE_TYPE ].nextState = &ns_VARIABLE_TYPE;
	pArray[st_VARIABLE_NAME ].nextState = &ns_VARIABLE_NAME;
	pArray[st_VARIABLE_INIT ].nextState = &ns_VARIABLE_INIT;
	pArray[st_BUFFER        ].nextState = &ns_BUFFER;
	pArray[st_BUFFER_TYPE   ].nextState = &ns_BUFFER_TYPE;
	pArray[st_BUFFER_NAME   ].nextState = &ns_BUFFER_NAME;
	pArray[st_BUFFER_SIZE   ].nextState = &ns_BUFFER_SIZE;
	pArray[st_BUFFER_INIT   ].nextState = &ns_BUFFER_INIT;
	pArray[st_POINTER       ].nextState = &ns_POINTER;
	pArray[st_POINTER_TYPE  ].nextState = &ns_POINTER_TYPE;
	pArray[st_POINTER_NAME  ].nextState = &ns_POINTER_NAME;
	pArray[st_POINTER_INIT  ].nextState = &ns_POINTER_INIT;
	pArray[st_LAST          ].nextState = &ns_LAST;
	
	
	return pArray;
}


struct str *buildStr(struct token *pToken){
	struct token *pCurrentToken;
	enum ParserStates state = st_NONE;
	
	if (pToken == NULL) return NULL;
	
	/* Attempt to initialize root structure. Root structure should be empty */
	struct str *pStr = initStr();
	struct str *pCurrentStr = pStr;
	if (pStr == NULL) {
		printf("Error, could not allocate root structure\n");
		return NULL;
	}
	
	/* Attempt to initialize FSM array */
	struct FSM_state *pArray = initFSM();
	if (pArray == NULL) {
		printf("Error, could not initialize array of fsm functions\n");
		freeStr(&pStr);
		return NULL;
	}
	
	/* Loop through tokens */
	for (pCurrentToken = pToken; pCurrentToken != NULL; pCurrentToken = pCurrentToken->pNext){
		/* Get current state */
		struct FSM_state *pCurrentState = pArray + state;
		
		/*!!!!! Check pCurrentStrfor null !!!!!*/
		
		/* Evaluate current state */
		pCurrentState->evaluate(&(pCurrentStr), pCurrentToken);
		
		/* Find next state */
		state = pCurrentState->nextState(pArray, pCurrentToken);
	}
	
	
	return pStr;
}


void es_NONE(struct str** ppStr, struct token* pToken){
	/* Outside of structure, do nothing */
}


void es_ERROR(struct str** ppStr, struct token* pToken){
	/* Error, display */
	print("Error in parsing token '%s'\n", pToken->pText);
}


void es_STRUCTURE(struct str** ppStr, struct token* pToken){
	/* Entering structure, create new structure to work with */
	*ppStr->pNext = initStr();
	if (*ppStr->pNext == NULL){
		printf("Error, can not allocate new str in es_STRUCTURE\n");
		*ppStr = NULL;
		return;
	}
	
	/* Increment pointer to next */
	*ppStr = *ppStr->pNext;
}


void es_STRUCTURE_NAME(struct str** ppStr, struct token* pToken){
	/* Setting structure name */
	(*ppStr)->pName = strdup(pToken->pText);
	if ((*ppStr)->pName == NULL){
		printf("Error, failed to get structure name in es_STRUCTURE_NAME\n");
		*ppStr = NULL;
		return;
	}
}


void es_STRUCTURE_END(struct str**, struct token*){
	/* End of structure, do nothing */
}


void es_VARIABLE(struct str**, struct token*){
	/* Start of variable field */
	
}


void es_VARIABLE_TYPE(struct str**, struct token*){
	/* */
}

void es_VARIABLE_NAME(struct str**, struct token*);
void es_VARIABLE_INIT(struct str**, struct token*);
void es_BUFFER(struct str**, struct token*);
void es_BUFFER_TYPE(struct str**, struct token*);
void es_BUFFER_NAME(struct str**, struct token*);
void es_BUFFER_SIZE(struct str**, struct token*);
void es_BUFFER_INIT(struct str**, struct token*);
void es_POINTER(struct str**, struct token*);
void es_POINTER_TYPE(struct str**, struct token*);
void es_POINTER_NAME(struct str**, struct token*);
void es_POINTER_INIT(struct str**, struct token*);
void es_LAST(struct str**, struct token*);
