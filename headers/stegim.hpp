#ifndef _STEGIM_HPP_
#define _STEGIM_HPP_

#include <iostream>
#include <fstream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "stegparser.hpp"

using namespace std;
using namespace cv;

class Stegim{
private:
	//classe que irá conter uma imagem associada com um arquivo
	class Steg{
	public:
		//---------------------------------------------------------
		string image_name;
		Mat img;
		unsigned int n_character;
		unsigned int file_size;
		FILE* file;

		//---------------------------------------------------------
		Steg(string image_name, Mat img, unsigned int n_character, FILE* file):
			image_name(image_name),img(img), n_character(n_character), file(file)
		{
			DEBUG("", 3);

			file_size = (this->file == NULL)?-1:file_length(this->file);

			DEBUG("Construído", 3);
		}

		//---------------------------------------------------------
		~Steg(){
			if(file) fclose(file);
		}


	private:
		//calcula o tamanho do arquivo
		unsigned int file_length(FILE *f){
			unsigned int pos;
			unsigned int end;

			pos = ftell (f);
			fseek (f, 0, SEEK_END);
			end = ftell (f);
			fseek (f, pos, SEEK_SET);

			return end;
		}
		
	};

	bool B, G, R;
	short n_least_significant_bit;
	unsigned int n_channels;

	unsigned char mask;
	steg_command comm;

	vector<Steg> stegs;

	void (Stegim::*command_function)(Steg&) = NULL;

	void steg(Steg& s);
	void info(Steg& s);
	void x(Steg& s);

public:
	Stegim(StegimArgs* args): 	B(args->B), G(args->G), R(args->R),
					n_least_significant_bit(args->n_least_significant_bit)
	{
		DEBUG("", 1);
		DEBUG("cria a máscara", 2);
		mask = 0xFF>>(8-n_least_significant_bit);
		comm = args->command;

		n_channels = B + G + R;

		DEBUG("cria arquivo default", 2);
		//arquivo default de cada comando:
		//steg  = entrada padrão
		//info  = nenhum
		//x	= saida padrão
		FILE* file = NULL;
		string mode;

		switch (comm){
			case NONE:
				cerr << "At least one command is required" << endl;
				exit(1);
				break;
			case STEG:
				command_function = &Stegim::steg;
				mode = "r";
				file = stdin;
				break;
			case X:
				command_function = &Stegim::x;
				mode = "w";
				file = stdout;
				break;
			case INFO:
				command_function = &Stegim::info;
				mode = "r";
				break;
		}
		if(args->file.size()){
			file = fopen(args->file.c_str(), mode.c_str());

			if(!file){
				cerr << "Can not open the file \"" << args->file << "\" !" << endl;
				exit(1);
			}
		}

		DEBUG("abre as imagens com a estrutura do opencv e associa na classe Steg", 2);
		for(unsigned int i=0; i<args->img.size(); i++){
			DEBUG("Imagem nº " << i+1, 3);
			Mat img = imread(args->img[i], CV_LOAD_IMAGE_COLOR);

			if(!img.data){
				cerr << "Can not open the image \"" << args->img[i] << "\"!" << endl;
				exit(1);
			}
			
			DEBUG("Calcula o número de caracteres que cabem na imagem", 3);
			unsigned int n_character =
				((img.cols*img.rows*n_channels*n_least_significant_bit)/8)-4;
			
			DEBUG("Adiciona no vetor stegs", 3);
			stegs.push_back(Steg(args->img[i], img, n_character, file));
			
		}
		
		DEBUG("Construído", 1);
	}

	void run(){
		DEBUG("RUN!", 1);
		if(command_function != NULL)
			for ( unsigned int i=0; i<stegs.size(); i++ )
				(*this.*command_function)(stegs[i]);
	}
};

//----------------------------------------------------------------
void Stegim::steg(Steg& s){
	cout << "steg (" << s.image_name << ")" << endl;
}

void print_mask(unsigned char mask){
	int shift = 8;
	while(shift--) cout << ((mask>>shift)&1);
	cout << endl;
}

void Stegim::info(Steg& s){
	cout << "> info (" << s.image_name << ")" << endl;
	cout << '\t' << "Resolution: " << s.img.size() << endl;
	cout << '\t' << "N bytes (character): " << s.n_character << endl;
	cout << '\t' << "Channel mask: ";
	print_mask(this->mask);

	//Se couber o arquivo na imagem
}

void Stegim::x(Steg& s){
	cout << "x (" << s.image_name << ")" << endl;
}

#endif //_STEGIM_HPP_
