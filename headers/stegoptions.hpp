/***********************************************************
 * stegoptions.hpp - Esse arquivo conterá todas as opções
 * disponíveis no stegim
 * ********************************************************/
#ifndef _STEGOPTIONS_HPP_
#define _STEGOPTIONS_HPP_

//--OPÇÕES-------------------------------------------------
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
//--STEGIM-------------------------------------------------
//VERBOSE
#define OPT_VERBOSE 		"verbose", 'v' //1 e 2
#define OPT_VERBOSE_ARG		0		//3
#define OPT_VERBOSE_DOC		"Enable verbose mode" //5
#define OPT_VERBOSE_GROUP	0	//6
#define VERBOSE_OPT(X)		{OPT_VERBOSE, OPT_VERBOSE_ARG, X, OPT_VERBOSE_DOC, OPT_VERBOSE_GROUP}

//QUIET
#define OPT_QUIET 		"quiet", 'q' //1 e 2
#define OPT_QUIET_ARG		0		//3
#define OPT_QUIET_DOC		"Disable verbose mode" //5
#define OPT_QUIET_GROUP		0		//6
#define QUIET_OPT(X)		{OPT_QUIET, OPT_QUIET_ARG, X, OPT_QUIET_DOC, OPT_QUIET_GROUP}

//--DEFAULT FOR COMMANDS-----------------------------------
//CHANNELS
#define OPT_CHANNELS 		"channels", 'C' //1 e 2
#define OPT_CHANNELS_ARG 	"BGR_STRING"	//2
#define OPT_CHANNELS_DOC 	"Sets the channels that will be used for the steganography using the BGR_STRING. e.g. \"BGR\" use all channels; \"B-R\" blue and red channels;"	//4
#define OPT_CHANNELS_GROUP	0		//5
#define CHANNELS_OPT(X) 	{OPT_CHANNELS, OPT_CHANNELS_ARG, X, OPT_CHANNELS_DOC, OPT_CHANNELS_GROUP}

//NLEAST-SIGNIFICANT-BIT
#define OPT_NLEAST_SB		"nleast-significant_bit", 'b'	//1 e 2
#define OPT_NLEAST_SB_ARG	"N"	//2
#define OPT_NLEAST_SB_DOC	"Sets the number of bits used on the stenography mask."	//4
#define OPT_NLEAST_SB_GROUP	0	//5
#define NLEAST_SB_OPT(X)	{OPT_NLEAST_SB, OPT_NLEAST_SB_ARG, X, OPT_NLEAST_SB_DOC, OPT_NLEAST_SB_GROUP}	//6

//ASCII FILE
#define OPT_FILE		"file", 'f' 	//1 e 2
#define OPT_FILE_ARG		"FILE[.txt]" 	//3
#define OPT_FILE_DOC 		"Hide ascii text from FILE in image (steg) or extract the hidden text to FILE (x) or show informations about the FILE and image(info)."  	//4
#define OPT_FILE_GROUP		0	//5
#define FILE_OPT(X)		{OPT_FILE, OPT_FILE_ARG, X, OPT_FILE_DOC, OPT_FILE_GROUP}

//--STEG---------------------------------------------------
	//output img
#define OPT_OUTFILE		"output-img", 'O' //1 e 2
#define OPT_OUTFILE_ARG		"img.<jpg|png|...>" //3
#define OPT_OUTFILE_DOC		"Write the image with hidden content in img.<jpg|png|...>. "\
				"Warning: in case of multiple images, the -A options is recommended."
#define OPT_OUTFILE_GROUP		0	//5
#define FILE_OUTFILE_OPT(X)	{OPT_OUTFILE, OPT_OUTFILE_ARG, X, OPT_OUTFILE_DOC, OPT_OUTFILE_GROUP}
	
	//append-to-output
#define OPT_APPEND_OUTPUT	"append-to-output", 'A' //1 e 2
#define OPT_APPEND_OUTPUT_ARG	"string"		//3
#define OPT_APPEND_OUTPUT_DOC	"Append `string` to the image output. "\
				"Ex: string = \"_to_append\"; "\
				"Image name = \"img.jpg\"; "\
				"The output will be = img_to_append.jpg. "\
				"Default string is \"_steg\""
#define OPT_APPEND_OUTPUT_GROUP 0
#define APPEND_OUTPUT_OPT(X) 	{OPT_APPEND_OUTPUT, OPT_APPEND_OUTPUT_ARG, X, OPT_APPEND_OUTPUT_DOC, OPT_APPEND_OUTPUT_GROUP}

//OPTIONS GROUPS-------------------------------------------

//DEFAULT OPTIONS (todos os comandos terão)
#define STEGIM_DEFAULT_OPTION(X)\
	QUIET_OPT(OPTION_HIDDEN), VERBOSE_OPT(OPTION_HIDDEN), CHANNELS_OPT(X), NLEAST_SB_OPT(X), FILE_OPT(X)

//STEG
#define STEG_NAME "steg"
#define STEGIM_STEG_OPTIONS(X) {0,0,0,0,"Command "STEG_NAME,0}, STEGIM_DEFAULT_OPTION(X),\
								FILE_OUTFILE_OPT(X), APPEND_OUTPUT_OPT(X)
//INFO
#define INFO_NAME "info"
#define STEGIM_INFO_OPTIONS(X) {0,0,0,0,"Command "INFO_NAME,0}, STEGIM_DEFAULT_OPTION(X)

//X
#define X_NAME "x"
#define STEGIM_X_OPTIONS(X) {0,0,0,0,"Command "X_NAME,0}, STEGIM_DEFAULT_OPTION(X)

//STEGIM
#define STEGIM_OPTIONS VERBOSE_OPT(0), QUIET_OPT(0),\
			STEGIM_STEG_OPTIONS(OPTION_NO_USAGE),\
			STEGIM_INFO_OPTIONS(OPTION_NO_USAGE),\
			STEGIM_X_OPTIONS(OPTION_NO_USAGE)


#endif //_STEGOPTIONS_HPP
