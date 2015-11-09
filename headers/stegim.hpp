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

//Canais
typedef enum { B, G, R } BGR ;

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

	//index dos canais
	size_t chcount[] = {0, 0, 0};
	{
		size_t i = 0;
		if(this->B) 	chcount[B] = i++;
		if(this->G) 	chcount[G] = i++;
		if(this->R) 	chcount[R] = i++;
	}

	size_t ibit 	= N_BIT_FOR_NCHARACTER;			//guarda o valor o iézimo bit que escondemos
	size_t nbit 	= this->n_least_significant_bit;	//número de bits menos significativos
	size_t nch 	= this->n_channels;			//número de canais

	while(size_t n_data = fread(buffer, sizeof(char), BUFFER_SIZE, input)){
		//Deu overflow?
		n_bytes_writed += n_data;
		if(n_bytes_writed > s.n_character)
			return SERROR_IMG_OVERFLOW;

		//Para todo byte lido...
		for(size_t i = 0; i<n_data; i++){
			short bit = 8;
			//para todo bit do byte lido...
			while(bit--){
				unsigned char b = (buffer[i]>>bit)&1;

				//Calcula valores de acordo com o ibit
				size_t lin 	= ibit/(nbit*nch*s.img.cols);
				size_t col 	= (ibit/(nch*nbit))%s.img.cols;
				size_t ch  	= (ibit/nbit)%nch;
				size_t bit_pos 	= ibit%nbit;

				//pega ponteiro apontado para a linha `lin`
				//da imagem de entrada
				uchar* img_ptr = s.img.ptr<uchar>(lin);
				//da imagem de saída
				uchar* out_ptr = output.ptr<uchar>(lin);

				//pega ponteiro apontado para (lin, col)
				//da imgem de entrada
				uchar* img_pixel = img_ptr + (col * nch);
				//da imagem de saída
				uchar* out_pixel = out_ptr + (col * nch);

				if(this->B && ch == chcount[B]){
					out_pixel[B] = img_pixel[B] | (b << (nbit - bit_pos));

				}else if(this->G && ch == chcount[G]){
					out_pixel[G] = img_pixel[G] | (b << (nbit - bit_pos));

				}else if(this->R && ch == chcount[R]){
					out_pixel[R] = img_pixel[R] | (b << (nbit - bit_pos));

				}

				ibit++;
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

	Mat output_img = s.img.clone();

	STEG_ERROR error;
	if(!(error = steg_in_image(s, output_img, text_input))){

		if(output_img.data){
			imwrite( imgout, output_img );
		}
		else
			cerr << "An error occurred while processing "
				<< s.image_name  << "." << endl;

		if(this->args->verbose)
			cout << imgout << " completed." << endl;

	}else{
		cerr << "Error occurred while processing " << imgout << ":\v";
		
		if(error == SERROR_IMG_OVERFLOW){
			cerr << imgout << " has no room for the input." << endl;
		}
	}

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
