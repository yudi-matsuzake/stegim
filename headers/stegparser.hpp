#ifndef _STEGPARSER_HPP_
#define _STEGPARSER_HPP_

//--INCLUDES-----------------------------------------------
#include <argp.h>

/* MACROS
 * ------
 *  *****/
#define STEGIM_TITLE 			"stegim"
#define STEGIM_VERSION 			"0.0"
#define STEGIM_PROGRAM_BUG_ADDRESS 	"<www.github.com/yudi-matsuzake/stegim>"
#define STEGIM_DOC\
	"stegim - ferramenta simples para esteganografia em imagens"

//--USAGES-------------------------------------------------
#define STEGIM_STEG_USAGE\
	"stegim steg [OPTIONS] <img.jpg> [img0.jpg ... ] ] [text.txt]\n"\
	"stegim steg --foreach-img-txt <img.jpg> <text.txt> [img.jpg text.txt ...]\n"\
	"stegim steg --nleast-significant-bit N <img.jpg> [img0.jpg ... ] ] [text.txt]\n"\
	"stegim steg --channels CHANNEL_STRING [img0.jpg ... ] ] [text.txt]\n"

#define STEGIM_INFO_USAGE\
	"stegim info [OPTIONS] <img.jpg>\n"\
	"stegim info --nleast-significant-bit N <img.jpg> [img0.jpg ...]\n"\
	"stegim info --n-channel N <img.jpg> [img0.jpg ...]\n"

#define STEGIM_X_USAGE\
	"oi"\
	"stegim x [OPTIONS] <img.jpg> [img1.jpg ... ]\n"\
	"stegim x -O saida.txt <img.jpg>\n"

#define STEGIM_USAGE\
	"stegim [stegim OPTIONS] [<command> [command OPTIONS] <img.jpg> [img0.jpg ... ] [text.txt]]\n"\
	STEGIM_STEG_USAGE STEGIM_INFO_USAGE STEGIM_X_USAGE

/***********************************************************
 * STEGIM_OPTIONS - macro com as opções do programa
 * ---------------------------------------------------------
 *
 *  As opções são nessa ordem:
 *  1 - (const char*)	opção longa (--opcao)
 *  2 - (int)		opção curta (-p)
 *  3 - (const char*)	Argumento da opção (--opcao=ARG)
 *  4 - (int)		FLAGS
 *  ( ver http://www.gnu.org/software/libc/manual/html_node/Argp-Option-Flags.html )
 *  5 - (char*)		String de documentação
 *  6 - (int)		Grupo
 *  *******************************************************/
#define STEGIM_DEFAULT_OPTION\
	/*--channels*/\
	{\
		"channels", 			/*1*/\
		'C',				/*2*/\
		"BGR_STRING", 			/*3*/\
		0,				/*4*/\
		"Sets the channels that will be used for the steganography using the BGR_STRING. e.g. \"BGR\" use all channels; B-R blue and red channels; ", /*5*/\
		0, /*6*/\
	},\
	/*--nleast-significant-bit*/\
	{\
		"nleast-significant-bit",	/*1*/\
		'b',				/*2*/\
		"N", 				/*3*/\
		0,				/*4*/\
		"Sets the number of bits used on the stenography mask.", /*5*/\
		0				/*6*/\
	}

//-STEG OPTIONS--------------------------------------------
static struct argp_option STEGIM_STEG_OPTIONS[] = {
	//STEG
	{0,0,0,0,"steg",0},
	STEGIM_DEFAULT_OPTION,
	//--foreach-img-txt
	{
		"foreach-img-txt",	//1
		'F',			//2
		0,			//3
		0,			//4
		"For every image, a file must to be given for input.", //5
		0			//6		
	},
	{0,0,0,0,0,0}
};


//-INFO OPTIONS--------------------------------------------
static struct argp_option STEGIM_INFO_OPTIONS[] = {
	{0,0,0,0,"info",0},
	STEGIM_DEFAULT_OPTION,

	{0,0,0,0,0,0}
};

//-X OPTIONS-----------------------------------------------
static struct argp_option STEGIM_X_OPTIONS[] = {
	{0,0,0,0,"x",0},
	STEGIM_DEFAULT_OPTION,

	{0,0,0,0,0,0}
};

//--STEGARGS CLASS  ---------------------------------------
class StegArgs{
public:
	bool B, G, R; 			//holds whether use the R or G or B channels
	short n_least_significant_bit; 	//holds the number of bits that will be used
};

//--STEGPARSER CLASS---------------------------------------
class StegParser{
public:
	//constructor & destructor
	StegParser(int argc, char* argv[]);
	~StegParser();
	
	//get a StegArgs class based on the argc and argv
	StegArgs get_args();
};

#endif //_STEGIM_HPP_
