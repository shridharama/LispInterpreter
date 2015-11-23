#ifndef SCANNER_H
#define SCANNER_H

class Scanner{
	char temp_buffer;
	bool is_temp_buffer_empty;
  public:
  	string GetAtom(char ch);
  	bool ValidateAtom(string);
	Token* GetNextToken();
	Scanner(){
		is_temp_buffer_empty = true;
	}
};

#endif //SCANNER_H

