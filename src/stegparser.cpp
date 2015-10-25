/**************************************************************
 * StegParser - Tratador de argumentos pro stegim
 * ************************************************************
 *
 **************************************************************/
#include <stdlib.h>
#include <error.h>
#include <argp.h>
#include "stegparser.hpp"

/* CONSTANTES */
const char *argp_program_version = STEGIM_VERSION;
const char *argp_program_bug_address = STEGIM_PROGRAM_BUG_ADDRESS;

/* Documentação */
static char stegim_doc[] = STEGIM_DOC;

/* ARGS */
static char stegim_usage[] = STEGIM_USAGE;

//--STEG PARSER--------------------------------------------
StegParser::StegParser(int argc, char* argv[]){
	
}

StegParser::~StegParser(){}
