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

#define STEGIM_ERROR_NO_IMAGE "At least one image argument is required"

//PARSER ERRORS
#define BGR_STRING_ERROR "invalid BGR_STRING\n"
#define NLEAST_SIGNIFICANT_BIT_ERROR "invalid argument for the least_significant_bit: must to be a number [1-8]\n"

//-STEG OPTIONS--------------------------------------------
static struct argp_option steg_options[] = {STEGIM_STEG_OPTIONS(0), {0,0,0,0,0,0}};

//-INFO OPTIONS--------------------------------------------
static struct argp_option info_options[] = {STEGIM_INFO_OPTIONS(0), {0,0,0,0,0,0}};

//-X OPTIONS-----------------------------------------------
static struct argp_option x_options[] = {STEGIM_X_OPTIONS(0), {0,0,0,0,0,0}};

//-STEGIM OPTIONS------------------------------------------
static struct argp_option stegim_options[] = {STEGIM_OPTIONS, {0,0,0,0,0,0}};

//--STEG PARSER--------------------------------------------
StegParser::StegParser(int argc, char** argv): args(NULL) {
	DEBUG("",1);

	this->argc = argc;
	this->argv = argv;

	DEBUG("constroi structs", 2);
	argp_constroy();

	DEBUG("parse", 2);
	parse();
	DEBUG("Construído",1);
}

StegParser::~StegParser(){
	if(args!=NULL)
		delete args;
}

StegimArgs* StegParser::get_args(){
	return args;
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

			//se existe um parser já, cria um com base nesse,
				//senão cria um totalmente novo
			if(steg_parser->args != NULL){
				StegimArgs* last_parser = steg_parser->args;
				steg_parser->args = new StegArgs(*last_parser);
				delete last_parser;
			}else{
				steg_parser->args = new StegArgs();
			}
			break;
		case INFO:
			comm_name = INFO_NAME;
			comm_argp = &steg_parser->info_argp;

			//se existe um parser já, cria um com base nesse,
				//senão cria um totalmente novo
			if(steg_parser->args != NULL){
				StegimArgs* last_parser = steg_parser->args;
				steg_parser->args = new InfoArgs(*last_parser);
				delete last_parser;
			}else{
				steg_parser->args = new InfoArgs();
			}
			break;
		case X:
			comm_name = X_NAME;
			comm_argp = &steg_parser->x_argp;

			//se existe um parser já, cria um com base nesse,
				//senão cria um totalmente novo
			if(steg_parser->args != NULL){
				StegimArgs* last_parser = steg_parser->args;
				steg_parser->args = new XArgs(*last_parser);
				delete last_parser;
			}else{
				steg_parser->args = new XArgs();
			}
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

	state->next += argc - 1;

}

//PARSERS
//stegim parser
error_t StegParser::stegim_parser(int key, char* arg, struct argp_state* state){
	DEBUG("", 3);
	DEBUG("a chave é " << key << " = (char)" << (char) key, 3);
	StegParser* parser = static_cast<StegParser*>(state->input);
	switch(key){
		//verbosity off
		case 'q':
			parser->args = new StegimArgs();
			parser->args->verbose = false;
			break;
		//verbosity on
		case 'v':
			parser->args = new StegimArgs();
			parser->args->verbose = true;
			break;
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

			break;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}


//--FUNÇÕES AUXILIARES-------------------------------------
void channel_arg_parser(StegParser* steg_parser, char* arg, struct argp_state* state){
	//ponteiro é nulo?
	if(!arg)
		argp_error(state, BGR_STRING_ERROR);
	int bgr_size = strlen(arg);
	
	//BGR_STRING deve ter tamanho 3
	if(bgr_size != 3)
		argp_error(state, BGR_STRING_ERROR);

	bool bool_BGR[3];
	char BGR[3] = {'B', 'G', 'R'};

	for(int i=0; i<bgr_size; i++){
		if(arg[i] == BGR[i])
			bool_BGR[i] = true;
		else if(arg[i] == '-')
			bool_BGR[i] = false;
		else
			argp_error(state, BGR_STRING_ERROR);
	}

	if( !bool_BGR[0] && !bool_BGR[1] && !bool_BGR[2] )
		argp_error(state, BGR_STRING_ERROR);
	
	steg_parser->get_args()->set_bgr(bool_BGR[0], bool_BGR[1], bool_BGR[2]);
}

void n_bit_parser(StegParser* steg_parser, char* arg, struct argp_state* state){
	//se -b não recebeu argumento
	if(!arg) argp_error(state, NLEAST_SIGNIFICANT_BIT_ERROR);
	int n_bit = atoi(arg);

	if(n_bit < 1 || n_bit  > 8)
		argp_error(state, NLEAST_SIGNIFICANT_BIT_ERROR);

	steg_parser->get_args()->set_nleast_significant_bit(n_bit);
}

//default options parser
void StegParser::default_options_parser(int key, char* arg, struct argp_state* state){
	DEBUG("", 3);
	StegParser* steg_parser = static_cast<StegParser*>(state->input);

	switch(key){
		//verbosity off
		case 'q':
			steg_parser->args->verbose = false;
			break;
	
		//verbosity on
		case 'v':
			steg_parser->args->verbose = true;
			break;

		//channels
		case 'C':
			channel_arg_parser(steg_parser, arg, state);
			break;

		//least significant bit
		case 'b':
			n_bit_parser(steg_parser, arg, state);
			break;
		
		//arquivo
		case 'f':
			if(arg) steg_parser->args->add_file(string(arg));
			break;

		//nenhuma imagem de argumetno
		case ARGP_KEY_NO_ARGS:
			cerr << STEGIM_ERROR_NO_IMAGE << endl;
			argp_usage(state);
			break;
	}
}

//is_default_opt
bool is_default_opt(int key){
	DEBUG("", 3);
	return 	key == 'C' || key == 'b' || key == 'v' || key == 'q' ||
		key == 'f' || key == ARGP_KEY_NO_ARGS;
}

//steg parser
error_t StegParser::steg_parser(int key, char* arg, struct argp_state* state){
	DEBUG("", 3);
	StegParser* steg_parser = static_cast<StegParser*>(state->input);
	StegArgs* steg_args = static_cast<StegArgs*>(steg_parser->args);

	if(is_default_opt(key))
		default_options_parser(key, arg, state);
	else switch(key){
		case 'O':
			if(arg) steg_args->output_img = arg;
			break;
		case 'A':
			if(arg) steg_args->append_to_output = arg;
			break;
		case ARGP_KEY_ARG:
			if(arg)	steg_parser->args->add_img(string(arg));
			break;
	}
	return 0;
}

//info parser
error_t StegParser::info_parser(int key, char* arg, struct argp_state* state){
	DEBUG("", 3);
	StegParser* steg_parser = static_cast<StegParser*>(state->input);

	if(is_default_opt(key))
		default_options_parser(key, arg, state);
	else switch(key){
		case ARGP_KEY_ARG:
			if(arg)	steg_parser->args->add_img(string(arg));
			break;
	}
	return 0;
}

//x parser
error_t StegParser::x_parser(int key, char* arg, struct argp_state* state){
	DEBUG("", 3);
	StegParser* steg_parser = static_cast<StegParser*>(state->input);

	if(is_default_opt(key))
		default_options_parser(key, arg, state);
	else switch(key){
		case ARGP_KEY_ARG:
			if(arg)	steg_parser->args->add_img(string(arg));
			break;
	}


	return 0;
}

void StegParser::parse(){
	DEBUG("", 3);
	argp_parse(&stegim_argp, argc, argv, ARGP_IN_ORDER, 0, this);
}

//--OPERATORS----------------------------------------------
std::ostream& operator<< (std::ostream& stream, const StegimArgs& args){
	DEBUG("", 3);
	//command
	string command;
	if(!args.command){
		command = "None";
	}else if(args.command == STEG){
		command = "steg";
	}else if(args.command == INFO){
		command = "info";
	}else if(args.command == X){
		command = "x";
	}else{
		command = "Unknown";
	}

	stream << "Command: " << command << endl;
	
	//Canais sendo usados
	stream << "Channels: ";
	stream << ((args.B)?"B ":"");
	stream << ((args.G)?"G ":"");
	stream << ((args.R)?"R ":"");
	stream << endl;

	//Bits menos significativos
	stream << "Least significant bits: " << args.n_least_significant_bit << endl;

	//file
	if(args.file.size())stream << "File: " << args.file << endl;

	//imagens
	//primeira
	if(args.img.size() > 0){
		stream << "Image(s): " << args.img[0];

		for(size_t i=1; i<args.img.size(); i++)
			stream << ", " << args.img[i];

		stream << "." << endl;
	}
	
	return stream;
}

std::ostream& operator<< (std::ostream& stream, const StegArgs& args){
	DEBUG("", 3);
	stream << static_cast<const StegimArgs&>(args);

	if(args.output_img.size())
		stream << "output img: " << args.output_img << endl;

	return stream;
}

std::ostream& operator<< (std::ostream& stream, const InfoArgs& args){
	DEBUG("", 3);
	stream << static_cast<const StegimArgs&>(args);

	return stream;
}

std::ostream& operator<< (std::ostream& stream, const XArgs& args){
	DEBUG("", 3);
	stream << static_cast<const StegimArgs&>(args);

	return stream;
}
