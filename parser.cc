#include <vector>

#include "common.h"
#include "parser.h"
#include "scanner.h"
#include "evaluator.h"


bool AreAllInnerNodesList(TreeNode * node){
	if(node->left == NULL && node->right == NULL)
		return true;
	if(node->is_list_node_ == false)
		return false;
	bool is_left_list = AreAllInnerNodesList(node->left);
	bool is_right_list = AreAllInnerNodesList(node->right);
	return is_left_list && is_right_list;
}

void PrettyPrintNode(TreeNode* node){
	if(node->left == NULL && node->right == NULL){
		cout<<node->atom_val_;
		return;
	}
	cout<<"(";
	PrettyPrintNode(node->left);
	cout<<" . ";
	PrettyPrintNode(node->right);
	cout<<")";
}

void PrettyPrintList(TreeNode *node){
	if(node->left == NULL && node->right == NULL){
		cout<<node->atom_val_;
		return;
	}
	cout<<"(";
	PrettyPrintList(node->left);
	
	while(node->right != NULL && (node->right)->is_internal_node_){
		node = node->right;
		cout<<" ";
		PrettyPrintList(node->left);
	}
	cout<<")";
}

void Parser::ParseStart(){
	vector<TreeNode*> RootNodes;
	TreeNode* root = new TreeNode();
	ParseSexp(root);
	//tree has been created by this time

	//evaluates and simplifies tree
	map< string, stack<TreeNode*> > A_list;
	TreeNode* new_root = evaluator.eval(root, A_list);

	if(AreAllInnerNodesList(new_root)){
		//PrettyPrintList(root);
		
		PrettyPrintList(new_root);

	} else{
		PrettyPrintNode(new_root);
	}


	//DeleteTree(root);
	cout<<endl;

	while(cin){
		TreeNode * root = new TreeNode();
		ParseSexp(root);
		//this needs to be checked as the Sexp parsed could consist of just eof
		if(root->atom_val_ != "null"){
			map< string, stack<TreeNode*> > A_list2;
			//evaluates and simplifies tree
			TreeNode* new_root = evaluator.eval(root, A_list2);

			if(AreAllInnerNodesList(new_root)){
				//PrettyPrintList(root);
				//cout<<endl;
				PrettyPrintList(new_root);

			} else{
				PrettyPrintNode(new_root);
			}
			//DeleteTree(root);
			cout<<endl;
		}
	}
}

void Parser::ParseSexp(TreeNode* node){

	Token * token = scanner.GetNextToken();
	//end of file check
	if (token == NULL){
		node->atom_val_ = "null";

  		return;
  	}
  	
  	TokenType token_type = token->GetType();
  	
  	if(token_type != kOpenParenthesis && token_type != kAtom){
		delete token;
  		throw "SExp doesn't start with ( or Atom.";
  	} else {
  		//sexp = atom
  		if(token_type == kAtom){
  			node->is_internal_node_ = false;
  			//TODO: Shift NIL to a constant string
  			if(token->GetValue() == "NIL"){
  				node->is_list_node_ = true;
  			}

  			node->atom_val_ = token->GetValue();
  		}
  		//sexp = (sexp.sexp) 
  		else{
  			TreeNode * left_node = new TreeNode();
  			node->left = left_node;
  			
  			ParseSexp(left_node);
  			
  			Token * token_dot = scanner.GetNextToken();
  			// null check needs to be done as it could be eof - ex: in case input is "(A"
  			if(token_dot == NULL || token_dot->GetType() != kDot){
  				
  				delete token_dot;
  				delete token;
  				throw "SExp does not have dot after it.";
  			
  			} else {
  				TreeNode * right_node = new TreeNode();
  				node->right = right_node;
  				
  				ParseSexp(right_node);

  				Token * token_sexp_2 = scanner.GetNextToken();
  				if(token_sexp_2 == NULL || token_sexp_2->GetType() != kClosingParenthesis){
	  				
	  				delete token_sexp_2;
	  				delete token_dot;
	  				delete token;
	  				throw "SExp does not end with closing parenthesis.";
  				
  				}

  				if(right_node->is_list_node_ == true){
  					node->is_list_node_ = true;
  				}

  			}
  		}
  	}

	
  	token = NULL;
}





