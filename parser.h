#ifndef PARSER_H
#define PARSER_H

#include "common.h"
#include "scanner.h"
#include "evaluator.h"

class Parser{
	Scanner scanner;
  Evaluator evaluator;

  public:
  	Parser() { }

	void ParseSexp(TreeNode* node);

	void ParseStart();

};


#endif //PARSER_H