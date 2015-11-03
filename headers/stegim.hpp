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
public:
	class Steg;
private:
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
	Stegim(StegimArgs* args): 	B(args->B), G(args->G), R(args->R),
					n_least_significant_bit(args->n_least_significant_bit)
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
void Stegim::steg(Steg& s){
	if(!default_steg_inicialize(s, "r")) return;
	cout << "steg (" << s.image_name << ")" << endl;
}

//--INFO----------------------------------------------------------
void print_mask(unsigned char mask){
	int shift = 8;
	while(shift--) cout << ((mask>>shift)&1);
	cout << endl;
}

void Stegim::info(Steg& s){
	if(!default_steg_inicialize(s, "r")) return;

	cout << "> info (" << s.image_name << ")" << endl;
	cout << '\t' << "Resolution: " << s.img.size() << endl;
	cout << '\t' << "N bytes (character): " << s.n_character << endl;
	cout << '\t' << "N channels: "		<< this->n_channels << endl;
	cout << '\t' << "Channel mask: ";
	print_mask(this->mask);

	DEBUG("Se couber o arquivo na imagem, avisa", 3);
	if(s.file != NULL){
		unsigned int file_size = s.file_length();
		cout << "----------" << endl;
		//tamanho do arquivo de texto:
		cout << '\t' << "size of \""<< s.file_name <<"\"(in bytes/character): "
			<< file_size << endl;

		//o texto cabe na imagem?
		string has = "";
		if(file_size > s.n_character){
			has = "not ";
		}
		cout << '\t' << "\"" << s.image_name <<
			"\" has " << has << "space for the content of \"" <<
			s.file_name << "\"! ";

		cout << "(" << file_size << "/" << s.n_character << ")" << endl;
	}

}

//--X-------------------------------------------------------------
void Stegim::x(Steg& s){
	if(!default_steg_inicialize(s, "w")) return;
	cout << "x (" << s.image_name << ")" << endl;
}

#endif //_STEGIM_HPP_
