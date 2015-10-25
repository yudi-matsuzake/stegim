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
	"stegim - A simple tool for ascii stenography images."

#define STEGIM_STEG_DOC\
	"stegimg steg - Command to realize the image stenography."

#define STEGIM_INFO_DOC\
	"stegimg info - Show some helpful informations about images or text."

#define STEGIM_X_DOC\
	"stegimg x - Command to \"extract\" the text from the image."

//--USAGES-------------------------------------------------
#define STEGIM_STEG_USAGE\
	"steg [OPTIONS] <img.jpg>\n"\
	"steg -F <img.jpg> <text.txt> [img.jpg text.txt ...]"\

#define STEGIM_INFO_USAGE\
	"info [OPTIONS] <img.jpg>\n"\
	"info -b N <img.jpg> [img0.jpg ...]"

#define STEGIM_X_USAGE\
	"x [OPTIONS] <img.jpg> [img1.jpg ... ]\n"\
	"x -O saida.txt <img.jpg>\n"\
	"x -C CHANNEL_STRING <img.jpg> [img0.jpg ...]"

#define STEGIM_USAGE\
	"[<command> [command OPTIONS] <img.jpg>]\n"\
	STEGIM_STEG_USAGE "\n" STEGIM_INFO_USAGE "\n" STEGIM_X_USAGE


//--STEGARGS CLASS  ---------------------------------------
class StegArgs{
public:
	bool B, G, R; 			//holds whether use the R or G or B channels
	short n_least_significant_bit; 	//holds the number of bits that will be used
};

//--STEGPARSER CLASS---------------------------------------
class StegParser{
private:
	int argc;
	char** argv;
	struct argp stegim_argp, steg_argp, info_argp, x_argp; //holds the argp's struct

	void argp_constroy();	//constroi the argp variables
	void parse();		//parses the argc and argv
public:
	//constructor & destructor
	StegParser(int argc, char* argv[]);
	~StegParser();
	
	//get a StegArgs class based on the argc and argv
	StegArgs get_args();
};

#endif //_STEGIM_HPP_
