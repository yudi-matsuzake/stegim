#include <iostream>
#include "stegparser.hpp"
#include "debug.hpp"

using namespace std;

int main(int argc, char* argv[]){
	DEBUG("Olá mundo! Eu sou o stegim!", 0);

	StegParser parser(argc, argv);

	DEBUG("Tchau mundo! Foi um prazer!", 0);
	return 0;
}
