#include "common.h"
#include "scanner.h"

#include <cctype>


string Scanner::GetAtom(char ch){
	string st = "";
	while(cin && (isdigit(ch) || isupper(ch) || ch== '+' || ch=='-')){
		st.push_back(ch);
		cin.get(ch);
	}
	
	is_temp_buffer_empty = false;
	temp_buffer = ch;
	
	return st;
}

bool Scanner::ValidateAtom(string atom_val){
	char first = atom_val[0];
	if(isupper(first))
		return true;

	for(unsigned int i=1;i<atom_val.length();i++){
		if(isupper(atom_val[i]))
			return false;
	}
	return true;
}

Token* Scanner::GetNextToken(){
	char ch;
	if(is_temp_buffer_empty){
		if(cin){
			cin.get(ch);
		}else{
			return NULL;
		}
	} else{
		ch = temp_buffer;
		is_temp_buffer_empty = true;
	}
	
	while(isspace(ch) && cin){
		cin.get(ch);
	}

	if(!cin)
		return NULL;
	
	Token* token;
	if(ch == '('){
		token = new Token(kOpenParenthesis);
	} else if(ch == ')'){
		token = new Token(kClosingParenthesis);
	} else if(ch == '.'){
		token = new Token(kDot);
	} else{
		string atom_val = GetAtom(ch);
		if(ValidateAtom(atom_val)){
			Atom* atom = new Atom(atom_val);
			return atom;
		} else {
			throw "Invalid atom.";
		}
	}
	return token;

}