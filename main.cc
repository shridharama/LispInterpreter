#include <iostream>
#include "common.h"
#include "parser.h"
#include "scanner.h"
using namespace std;

int main() {

  	Parser parser;
  	try{
  		parser.ParseStart();
	} catch (char const * error){
		cout<<"ERROR: "<<error<<endl;
	}

}