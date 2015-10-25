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

//-STEG OPTIONS--------------------------------------------
static struct argp_option steg_options[] = {STEGIM_STEG_OPTIONS, {0,0,0,0,0,0}};

//-INFO OPTIONS--------------------------------------------
static struct argp_option info_options[] = {STEGIM_INFO_OPTIONS, {0,0,0,0,0,0}};

//-X OPTIONS-----------------------------------------------
static struct argp_option x_options[] = {STEGIM_X_OPTIONS, {0,0,0,0,0,0}};

//-STEGIM OPTIONS------------------------------------------
static struct argp_option stegim_options[] = {STEGIM_STEG_OPTIONS, STEGIM_INFO_OPTIONS, STEGIM_X_OPTIONS, {0,0,0,0,0,0}};

/* Documentação */
//static char stegim_doc[] = STEGIM_DOC;
//
///* ARGS */
//static char stegim_usage[] = STEGIM_USAGE;

//--PROTÓTIPOS---------------------------------------------

//--STEG PARSER--------------------------------------------
StegParser::StegParser(int argc, char** argv){
	DEBUG("",1);

	this->argc = argc;
	this->argv = argv;

	DEBUG("constroi structs", 2);
	argp_constroy();

	DEBUG("parse", 2);
	parse();
}

StegParser::~StegParser(){}


//--ARGP_CONSTROY------------------------------------------
/* Constrói os argp's
 * *********************
 * A struct argp é formada por:
 *
 * 1 (argp_option[])        -	Um vetor com as opções
 *
 * 2 (argp_parser_t parser) - 	Um ponteiro pra uma função que irá parsear
 * 				as opções
 *
 * 3 (char*)		    -	Documentação de usage
 *
 * 4 (char*) 		    -	Documentação da função
 *
 * 5 (argp_child*)	    -	Array para as opções filhas
 *
 * 6 help_finder
 *
 * 7 arg_domain
 * ********************/
void StegParser::argp_constroy(){
	DEBUG("Construindo STEG ARGP...", 2);
	this->steg_argp.options = steg_options; 	//1
	this->steg_argp.parser = 0;			//2
	this->steg_argp.args_doc = STEGIM_STEG_USAGE;	//3
	this->steg_argp.doc = STEGIM_STEG_DOC;		//4
	this->steg_argp.children = 0;			//5
	this->steg_argp.help_filter = 0;		//6
	this->steg_argp.argp_domain = 0;		//7

	DEBUG("Construindo STEG INFO...", 2);
	this->info_argp.options = info_options; 	//1
	this->info_argp.parser = 0;			//2
	this->info_argp.args_doc = STEGIM_INFO_USAGE;	//3
	this->info_argp.doc = STEGIM_INFO_DOC;		//4
	this->info_argp.children = 0;			//5
	this->info_argp.help_filter = 0;		//6
	this->info_argp.argp_domain = 0;		//7

	DEBUG("Construindo STEG X...", 2);
	this->x_argp.options = x_options; 		//1
	this->x_argp.parser = 0;			//2
	this->x_argp.args_doc = STEGIM_X_USAGE;		//3
	this->x_argp.doc = STEGIM_X_DOC;		//4
	this->x_argp.children = 0;			//5
	this->x_argp.help_filter = 0;			//6
	this->x_argp.argp_domain = 0;			//7

	
	DEBUG("Construindo STEGIM...", 2);
	this->stegim_argp.options = stegim_options; 	//1
	this->stegim_argp.parser = 0;			//2
	this->stegim_argp.args_doc = STEGIM_USAGE;	//3
	this->stegim_argp.doc = STEGIM_DOC;		//4
	this->stegim_argp.children = 0;			//5
	this->stegim_argp.help_filter = 0;		//6
	this->stegim_argp.argp_domain = 0;		//7
}

void StegParser::parse(){
	DEBUG("", 2);
	argp_parse(&stegim_argp, argc, argv, 0, 0, 0);
}
