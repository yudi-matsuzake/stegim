#include <iostream>
#include "stegparser.hpp"
#include "debug.hpp"

using namespace std;

int main(int argc, char* argv[]){
	DEBUG("Olá mundo! Eu sou o stegim!", 0);
	DEBUG("argc: " << argc, 1);

	StegParser parser(argc, argv);

	StegimArgs* args = parser.get_args();
	
	if(args != NULL && args->verbose)
		cout << *args;

	DEBUG("Tchau mundo! Foi um prazer!", 0);
	return 0;
}
