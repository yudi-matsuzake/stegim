#ifndef _STEGPARSER_HPP_
#define _STEGPARSER_HPP_

//--INCLUDES-----------------------------------------------
#include <argp.h>
#include "stegoptions.hpp"
#include "debug.hpp"

/* MACROS
 * ------
 *  *****/
#define STEGIM_TITLE 			"stegim"
#define STEGIM_VERSION 			STEGIM_TITLE " 0.0"
#define STEGIM_PROGRAM_BUG_ADDRESS 	"<www.github.com/yudi-matsuzake/stegim>"
#define STEGIM_DOC\
	"stegim - A simple tool to hide ascii text in images."

#define STEGIM_STEG_DOC\
	"stegimg steg - Command to realize the image stenography."

#define STEGIM_INFO_DOC\
	"stegimg info - Show some helpful informations about images or text."

#define STEGIM_X_DOC\
	"stegimg x - Command to \"extract\" the text from the image."

//--USAGES-------------------------------------------------
#define STEGIM_STEG_USAGE\
	"[OPTIONS] <img.jpg>\n"\
	"-F <img.jpg> <text.txt> [img.jpg text.txt ...]"\

#define STEGIM_INFO_USAGE\
	"[OPTIONS] <img.jpg>\n"\
	"-b N <img.jpg> [img0.jpg ...]"

#define STEGIM_X_USAGE\
	"[OPTIONS] <img.jpg> [img1.jpg ... ]\n"\
	"-O saida.txt <img.jpg>\n"\
	"-C CHANNEL_STRING <img.jpg> [img0.jpg ...]"

#define STEGIM_USAGE\
	"[<command> [command OPTIONS] <img.jpg>]"


//--STEGIM ARGS CLASSES---------------------------------------
typedef enum { NONE, STEG, INFO, X } steg_command;
class StegimArgs{
public:
	StegimArgs(steg_command command = NONE):B(true), G(true), R(true),
						verbose(false),
						n_least_significant_bit(2){
		this->command = command;
	}


	~StegimArgs(){}

	steg_command command; 	//Guarda qual comando foi acionado
	bool B, G, R; 			//Guarda os canais que serão usado para a esteganografia
	bool verbose;			//Guarda se o programa vai executar no modo verboso ou não
	short n_least_significant_bit; 	//Número de bits menos significativos que serão usados

	//set bgr
	void set_bgr(bool B, bool G, bool R);

	//operators
	friend std::ostream& operator<< (std::ostream& stream, const StegimArgs& args);
};

//--STEG ARGS----------------------------------------------
class StegArgs: public StegimArgs{
public:
	StegArgs():StegimArgs(STEG){}
	~StegArgs(){}

	//operators
	friend std::ostream& operator<< (std::ostream& stream, const StegArgs& args);
};

//--INFO ARGS----------------------------------------------
class InfoArgs: public StegimArgs{
public:
	InfoArgs():StegimArgs(INFO){}
	~InfoArgs(){}

	//operators
	friend std::ostream& operator<< (std::ostream& stream, const InfoArgs& args);
};

//--X ARGS----------------------------------------------
class XArgs: public StegimArgs{
public:
	XArgs():StegimArgs(X){}
	~XArgs(){}

	//operators
	friend std::ostream& operator<< (std::ostream& stream, const XArgs& args);
};

//--STEGPARSER CLASS---------------------------------------
class StegParser{
private:
	int argc;
	char** argv;
	struct argp stegim_argp, steg_argp, info_argp, x_argp; //holds the argp's struct

	void argp_constroy();	//Constrói as variáveis do argp
	void parse();		//Parseia os comandos

	StegimArgs* args;

	static void cmd_scheduler(struct argp_state* state, steg_command comm);

	//PARSERS for each command (stegim inclued)
	static void default_options_parser(int key, char* arg, struct argp_state* state);
	static error_t stegim_parser(int key, char* arg, struct argp_state* state);
	static error_t steg_parser(int key, char* arg, struct argp_state* state);
	static error_t info_parser(int key, char* arg, struct argp_state* state);
	static error_t x_parser(int key, char* arg, struct argp_state* state);
public:
	//constructor & destructor
	StegParser(int argc, char* argv[]);
	~StegParser();

	
	//Retorna a classe StegimArgs baseada nos argumentos dados
	StegimArgs* get_args();
};

#endif //_STEGIM_HPP_
