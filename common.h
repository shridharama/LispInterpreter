#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <string>
#include <map>



using namespace std;

enum ArithmeticOp{
	kPlus = 0,
	kMinus,
	kTimes,
	kQuotient,
	kRemainder
};

enum TokenType{
	kAtom,
	kOpenParenthesis,
	kClosingParenthesis,
	kDot,
	kInvalid
};

class Token{

  protected:
	TokenType type;
	
	string value;
  public:
  	bool isAtom;

  	Token(){
  	}

	Token(TokenType token_type){
		this->type = token_type;
		if(token_type != kAtom)
			isAtom = false;
		else
			isAtom = true;
	}
	
	virtual ~Token(){

	}

	TokenType GetType(){
		return type;
	}

	virtual string GetValue(){
		return "NON_ATOM_TOKEN_VALUE";
	}

};

class Atom : public Token{

	
  public:
  	
	Atom(string val): Token(kAtom){

		value = val;
		
	}

	~Atom(){

	}

	string GetValue(){
		return value;
	}

};

class TreeNode{
  public:
  	bool is_list_node_;
  	bool is_internal_node_;
  	string atom_val_;

  	TreeNode *left;
  	TreeNode *right;

  	TreeNode(){
  		left = NULL;
  		right = NULL;
  		is_list_node_ = false;
  		is_internal_node_ = true;
  		atom_val_ = "";
  	}

  	~TreeNode(){
  		left = NULL;
  		right = NULL;
  	}
};

inline void DeleteTree(TreeNode* node){
	if(node == NULL){
		return;
	}

	TreeNode* left = node->left;
	TreeNode *right = node->right;
	node->left = NULL;
	node->right = NULL;
	delete node;
	DeleteTree(left);
	DeleteTree(right);

}




#endif //COMMON_H