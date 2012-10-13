/* For parsing a string of tokens into a structure */

#ifndef PARSER_H
#define PARSER_H

#include "loader.h"
#include "structure.h"


/* States that the parser can be in */
enum ParserStates{
	st_NONE  = 0,
	st_ERROR = 1,
	
	/* Structure-specific states */
	st_STRUCTURE      = 2,
	st_STRUCTURE_NAME = 3,
	st_STRUCTURE_END  = 4,
	
	/* Variable-specific states */
	st_VARIABLE      = 5,
	st_VARIABLE_TYPE = 6,
	st_VARIABLE_NAME = 7,
	st_VARIABLE_INIT = 8,
	
	/* Buffer-specific states */
	st_BUFFER      =  9,
	st_BUFFER_TYPE = 10,
	st_BUFFER_NAME = 11,
	st_BUFFER_SIZE = 12,
	st_BUFFER_INIT = 13,
	
	/* Pointer-specific states */
	st_POINTER      = 14,
	st_POINTER_TYPE = 15,
	st_POINTER_NAME = 16,
	st_POINTER_INIT = 17,
	
	st_LAST = 18
};


struct FSM_state{
	/* Handle to operate on current structure node from a given token*/
	void (*evaluate)(struct str**, struct token*);
	
	/* Returns next state to jump to, given passed set of states */
	enum ParserStates (*nextState)(struct FSM_state*, struct token*);
};


/* Returns initialized list of fsm states */
struct FSM_state *initFSM();


/* Loads token list into structure list */
struct str *buildStr(struct token *pToken);


/* State evaluation actions */
void es_NONE(struct str**, struct token*);
void es_ERROR(struct str**, struct token*);
void es_STRUCTURE(struct str**, struct token*);
void es_STRUCTURE_NAME(struct str**, struct token*);
void es_STRUCTURE_END(struct str**, struct token*);
void es_VARIABLE(struct str**, struct token*);
void es_VARIABLE_TYPE(struct str**, struct token*);
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

/* Next-state finders */
enum ParserStates ns_NONE(struct FSM_state*, struct token*);
enum ParserStates ns_ERROR(struct FSM_state*, struct token*);
enum ParserStates ns_STRUCTURE(struct FSM_state*, struct token*);
enum ParserStates ns_STRUCTURE_NAME(struct FSM_state*, struct token*);
enum ParserStates ns_STRUCTURE_END(struct FSM_state*, struct token*);
enum ParserStates ns_VARIABLE(struct FSM_state*, struct token*);
enum ParserStates ns_VARIABLE_TYPE(struct FSM_state*, struct token*);
enum ParserStates ns_VARIABLE_NAME(struct FSM_state*, struct token*);
enum ParserStates ns_VARIABLE_INIT(struct FSM_state*, struct token*);
enum ParserStates ns_BUFFER(struct FSM_state*, struct token*);
enum ParserStates ns_BUFFER_TYPE(struct FSM_state*, struct token*);
enum ParserStates ns_BUFFER_NAME(struct FSM_state*, struct token*);
enum ParserStates ns_BUFFER_SIZE(struct FSM_state*, struct token*);
enum ParserStates ns_BUFFER_INIT(struct FSM_state*, struct token*);
enum ParserStates ns_POINTER(struct FSM_state*, struct token*);
enum ParserStates ns_POINTER_TYPE(struct FSM_state*, struct token*);
enum ParserStates ns_POINTER_NAME(struct FSM_state*, struct token*);
enum ParserStates ns_POINTER_INIT(struct FSM_state*, struct token*);
enum ParserStates ns_LAST(struct FSM_state*, struct token*);


#endif
