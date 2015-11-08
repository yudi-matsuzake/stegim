#ifndef _STEGIM_HPP_
#define _STEGIM_HPP_

#include <iostream>
#include <cstdio>
#include <fstream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "stegparser.hpp"

#define APPEND_TO_OUTPUT 	"_steg"
#define BUFFER_SIZE		8192
#define N_BIT_FOR_NCHARACTER	32

using namespace std;
using namespace cv;

//erros de steg
typedef enum { SERROR_NO_ERROR, SERROR_IMG_OVERFLOW } STEG_ERROR;

class Stegim{
public:
	class Steg;
private:
	StegimArgs* args;
	bool B, G, R;
	short n_least_significant_bit;
	bool verbose;
	unsigned int n_channels;

	unsigned char mask;
	steg_command comm;

	vector<Steg> stegs;

	void (Stegim::*command_function)(Steg&) = NULL;

	//Funções do steg
	STEG_ERROR steg_in_image(Stegim::Steg& s, Mat& output, FILE* input);
	void steg(Steg& s);

	//Funções do Info
	void info(Steg& s);

	//Funções do X
	void x(Steg& s);

public:
	//classe que irá conter uma imagem associada com um arquivo
	class Steg{
	public:
		//---------------------------------------------------------
		string image_name;
		string file_name;
		Mat img;
		FILE* file;
		unsigned int n_character;
		unsigned int file_size;

		//---------------------------------------------------------
		Steg(string image_name, string file_name):
			image_name(image_name), file_name(file_name), file(NULL),
			n_character(0), file_size(0)
		{}

		//---------------------------------------------------------
		~Steg(){
			if(file) fclose(file);
		}

		//calcula o tamanho do arquivo
		unsigned int file_length(){
			if(file){
				unsigned int pos;
				unsigned int end;

				pos = ftell (file);
				fseek (file, 0, SEEK_END);
				end = ftell (file);
				fseek (file, pos, SEEK_SET);

				return end;
			}
			return 0;
		}
	};
	//---------------------------------------------------------
	int default_steg_inicialize(Steg& s, string mode);

	//---------------------------------------------------------
	Stegim(StegimArgs* args): 	args(args),
					B(args->B), G(args->G), R(args->R),
					n_least_significant_bit(args->n_least_significant_bit),
					verbose(args->verbose)
	{
		DEBUG("", 1);
		DEBUG("cria a máscara", 2);
		mask = 0xFF>>(8-n_least_significant_bit);
		comm = args->command;

		n_channels = B + G + R;

		DEBUG("populando command_function", 2);
		switch (comm){
			case NONE:
				cerr << "At least one command is required" << endl;
				exit(1);
				break;
			case STEG:
				command_function = &Stegim::steg;
				break;
			case X:
				command_function = &Stegim::x;
				break;
			case INFO:
				command_function = &Stegim::info;
				break;
		}

		DEBUG("Cria Steg's a partir do vetor das imagens de StegimArg", 2);
		for(unsigned int i=0; i<args->img.size(); i++){
			DEBUG("Imagem nº " << i+1, 3);
			stegs.push_back(Steg(args->img[i], args->file));
		}
		
		DEBUG("Construído", 1);
	}

	//---------------------------------------------------------
	void run(){
		DEBUG("RUN!", 1);
		if(command_function != NULL)
			for ( unsigned int i=0; i<stegs.size(); i++ )
				(*this.*command_function)(stegs[i]);
	}
};

//----------------------------------------------------------------
int Stegim::default_steg_inicialize(Steg& s, string mode){
	s.img = imread(s.image_name, CV_LOAD_IMAGE_COLOR);
	if(!s.img.data){
		cerr << "Can not open image \"" << s.image_name << "\"!" << endl;
		return 0;
	}

	s.n_character = ((s.img.cols*s.img.rows*this->n_channels*this->n_least_significant_bit)/8)-4;

	if(s.file_name.size()){
		s.file = fopen(s.file_name.c_str(), mode.c_str());
		
		if(!s.file){
			cerr << "Can not open the file \"" << s.file_name << "\"!" << endl;
			return 0;
		}
	}else{
		s.file = NULL;
	}

	return 1;
}

//--STEG----------------------------------------------------------
// retorna o nome da imagem de saída
	// dado o nome da imagem "imagem.jpg" vai retornar "imagem-steg.jpg"
string img_out_name ( string img_name, string append_to_output = APPEND_TO_OUTPUT ){
	//posição do último ponto antes do fim da string
	size_t dot_position = img_name.rfind('.');
	
	// pega o formato ( ex: .jpg )
	string format = img_name.substr(dot_position, img_name.size() - dot_position);
	// pega o nome da string (ex: imagem
	string file_name = img_name.substr(0, dot_position);

	// retorna imagem_steg.jpg
	return (file_name + append_to_output + format);
}

//this is why we are here
STEG_ERROR Stegim::steg_in_image(Steg& s, Mat& output, FILE* input){
	DEBUG("", 3);

	char buffer[BUFFER_SIZE];		//buffer utilizado para ler o input
	unsigned long long n_bytes_writed = 0;	//número de bytes lidos para verificar overflow

	short nbit = this->n_least_significant_bit;

	//esconde o texto
	//indice da imagem que estamos escondendo
	unsigned int bimdex = N_BIT_FOR_NCHARACTER%nbit; 		//index do bit da imagem

	//index do pixel da imagem
		//essa coisa estranha [ +(bimtex != 0) ]
		// é como se fosse um arredondamento pro teto
	size_t imdex = (N_BIT_FOR_NCHARACTER/nbit) + (bimdex != 0);

	while(size_t n_data = fread(buffer, sizeof(char), BUFFER_SIZE, input)){
		//Deu overflow?
		n_bytes_writed += n_data;
		if(n_bytes_writed > s.n_character)
			return SERROR_IMG_OVERFLOW;

		//Para todo byte lido...
		for(size_t i = 0; i<n_data; i++){
			short bit = 8;
			//para todo bit de texto
			while(bit--){

			}
		}
	}

	//esconde o número de bytes
	
	return SERROR_NO_ERROR;
}

void Stegim::steg(Steg& s){
	DEBUG("", 2);
	if(!default_steg_inicialize(s, "r")) return;
	StegArgs* args = static_cast<StegArgs*>(this->args);

	//Image de saída
	string imgout;
	if(args->output_img.size()){
		imgout = args->output_img;
	}else{
		if(args->append_to_output.size())
			imgout = img_out_name(s.image_name, args->append_to_output);
		else
			imgout = img_out_name(s.image_name);
	}

	//Entrada de texto
	FILE* text_input = (s.file)?s.file:stdin;
	if(!s.file) s.file_name = "Standard Input";

	if( this->verbose )
		cout << "steg (" << s.image_name <<
			   	" + " << s.file_name <<
				" = "  << imgout <<
				")" << endl;
	
	if(text_input != stdin){
		if(s.n_character < s.file_length()){
			cerr << "The image " << s.image_name
				<< " has no room for " << s.file_name
				<< endl;
			return;
		}
	}

//	Mat output_img(s.img.size(), s.img.type());
//
//	STEG_ERROR error;
//	if(!(error = steg_in_image(s, output_img, text_input))){
//		if(this->args->verbose)
//			cout << imgout << " completed." << endl;
//
//	}else{
//		cerr << "Error occurred while processing " << imgout << ":\v";
//		
//		if(error == SERROR_IMG_OVERFLOW){
//			cerr << imgout << " has no room for the input." << endl;
//		}
//	}

}

//--INFO----------------------------------------------------------
void print_mask(unsigned char mask){
	int shift = 8;
	while(shift--) cout << ((mask>>shift)&1);
	cout << endl;
}

void Stegim::info(Steg& s){
	DEBUG("", 2);
	if(!default_steg_inicialize(s, "r")) return;

	cout << "> info (" << s.image_name << ")----------" << endl;
	cout << '\t' << "Resolution: " << s.img.size() << endl;
	cout << '\t' << "N bytes (character): " << s.n_character << endl;
	cout << '\t' << "N channels: "		<< this->n_channels << endl;
	cout << '\t' << "Channel mask: ";
	print_mask(this->mask);
	cout << '\t' << "Channel ~mask: ";
	print_mask(~this->mask);

	DEBUG("Se couber o arquivo na imagem, avisa", 3);
	if(s.file != NULL){
		unsigned int file_size = s.file_length();
		//tamanho do arquivo de texto:
		cout << '\t' << "size of \""<< s.file_name <<"\"(in bytes/character): "
			<< file_size << endl;

		//o texto cabe na imagem?
		string has = "";
		if(file_size > s.n_character){
			has = "no ";
		}
		cout << '\t' << "\"" << s.image_name <<
			"\" has " << has << "room for the content of \"" <<
			s.file_name << "\"! ";

		cout << "(" << file_size << "/" << s.n_character << ")" << endl;
	}

}

//--X-------------------------------------------------------------
void Stegim::x(Steg& s){
	DEBUG("", 2);
	if(!default_steg_inicialize(s, "w")) return;
	cout << "x (" << s.image_name << ")" << endl;
}

#endif //_STEGIM_HPP_
