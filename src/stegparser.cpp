/**************************************************************
 * StegParser - Tratador de argumentos pro stegim
 * ************************************************************
 *
 **************************************************************/
#include <stdlib.h>
#include <error.h>
#include <argp.h>
#include <string.h>
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
static struct argp_option stegim_options[] = {STEGIM_OPTIONS, {0,0,0,0,0,0}};

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

StegimArgs StegParser::get_args(){
	return *args;
}

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
	this->steg_argp.parser = steg_parser;		//2
	this->steg_argp.args_doc = STEGIM_STEG_USAGE;	//3
	this->steg_argp.doc = STEGIM_STEG_DOC;		//4
	this->steg_argp.children = 0;			//5
	this->steg_argp.help_filter = 0;		//6
	this->steg_argp.argp_domain = 0;		//7

	DEBUG("Construindo STEG INFO...", 2);
	this->info_argp.options = info_options; 	//1
	this->info_argp.parser = info_parser;			//2
	this->info_argp.args_doc = STEGIM_INFO_USAGE;	//3
	this->info_argp.doc = STEGIM_INFO_DOC;		//4
	this->info_argp.children = 0;			//5
	this->info_argp.help_filter = 0;		//6
	this->info_argp.argp_domain = 0;		//7

	DEBUG("Construindo STEG X...", 2);
	this->x_argp.options = x_options; 		//1
	this->x_argp.parser = x_parser;			//2
	this->x_argp.args_doc = STEGIM_X_USAGE;		//3
	this->x_argp.doc = STEGIM_X_DOC;		//4
	this->x_argp.children = 0;			//5
	this->x_argp.help_filter = 0;			//6
	this->x_argp.argp_domain = 0;			//7

	
	DEBUG("Construindo STEGIM...", 2);
	this->stegim_argp.options = stegim_options; 	//1
	this->stegim_argp.parser = stegim_parser;	//2
	this->stegim_argp.args_doc = STEGIM_USAGE;	//3
	this->stegim_argp.doc = STEGIM_DOC;		//4
	this->stegim_argp.children = 0;			//5
	this->stegim_argp.help_filter = 0;		//6
	this->stegim_argp.argp_domain = 0;		//7
}

//Scheduler
void StegParser::cmd_scheduler( struct argp_state* state, steg_command comm){
	DEBUG("", 2);
	int    argc = state->argc - state->next + 1;
	char** argv = &state->argv[state->next - 1];
	char*  argv0 =  argv[0];

	const char* comm_name = NULL; //nome do comando
	struct argp * comm_argp = NULL;
	StegParser* steg_parser = static_cast<StegParser*>(state->input);
	switch (comm){
		case STEG:
			comm_name = STEG_NAME;
			comm_argp = &steg_parser->steg_argp;
			break;
		case INFO:
			comm_name = INFO_NAME;
			comm_argp = &steg_parser->info_argp;
			break;
		case X:
			comm_name = X_NAME;
			comm_argp = &steg_parser->x_argp;
			break;
		default:
			argp_error(state, "%x command is not valid\n", comm);
	}
	DEBUG("O nome do comando é:" << comm_name, 3);

	DEBUG("Alocando e gerenciado o argv0", 3);
	argv[0] = (char*) malloc(strlen(argv0) + strlen(comm_name) + 1);
	sprintf(argv[0], "%s %s", STEGIM_TITLE, comm_name);

	DEBUG("Chamando argparser para " << comm_name, 3);
	argp_parse(comm_argp, argc, argv, ARGP_IN_ORDER, 0, steg_parser);

	free (argv[0]);


	argv[0] = argv0;

}

//PARSERS
//stegim parser
error_t StegParser::stegim_parser(int key, char* arg, struct argp_state* state){
	switch(key){
		case ARGP_KEY_ARG:
			if(arg != NULL){

				//STEG command
				if(strcmp(arg, STEG_NAME) == 0)
					cmd_scheduler(state, STEG);
				//INFO command
				else if(strcmp(arg, INFO_NAME) == 0)
					cmd_scheduler(state, INFO);
				//X command
				else if(strcmp(arg, X_NAME) == 0)
					cmd_scheduler(state, X);
				else
					argp_error(state, "%s is not a valid command.", arg);

			}

		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

//steg parser
error_t StegParser::steg_parser(int key, char* arg, struct argp_state* state){
	StegParser* steg_parser = static_cast<StegParser*>(state->input);
	return 0;
}

//info parser
error_t StegParser::info_parser(int key, char* arg, struct argp_state* state){
	StegParser* steg_parser = static_cast<StegParser*>(state->input);
	return 0;
}

//x parser
error_t StegParser::x_parser(int key, char* arg, struct argp_state* state){
	StegParser* steg_parser = static_cast<StegParser*>(state->input);
	return 0;
}

void StegParser::parse(){
	DEBUG("", 2);
	argp_parse(&stegim_argp, argc, argv, ARGP_IN_ORDER, 0, this);
}
