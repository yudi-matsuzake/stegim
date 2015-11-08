#include <iostream>
#include "stegparser.hpp"
#include "stegim.hpp"
#include "debug.hpp"

using namespace std;

int main(int argc, char* argv[]){
	DEBUG("Olá mundo! Eu sou o stegim!", 0);
	DEBUG("argc: " << argc, 1);

	StegParser parser(argc, argv);
	StegimArgs* args = parser.get_args();
	
	//se for no modo verboso, mostra agumentos
	if(args != NULL && args->verbose){
		if(args->command == NONE) cout << *args ;
		else if(args->command == STEG) cout << *static_cast<StegArgs*>(args);
		else if(args->command == INFO) cout << *static_cast<InfoArgs*>(args);
		else if(args->command == X) cout << *static_cast<XArgs*>(args);
	}

	//stegim
	if(args != NULL){
		Stegim stegim(args);
		stegim.run();
	}

	DEBUG("Tchau mundo! Foi um prazer!", 0);
	return 0;
}
