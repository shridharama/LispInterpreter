#include "evaluator.h"
#include "common.h"
#include <cstdlib>
#include <cctype>
#include <cstring>
#include <string>
#include <sstream>
#include <vector>
#include <stack>
#include <set>

map<string, TreeNode*> D_list;

string key_words[] = {"T","NIL","CAR","CDR","CONS","ATOM","EQ","NULL","INT","PLUS","MINUS","TIMES","QUOTIENT","REMAINDER","LESS","GREATER","COND","QUOTE"};

set<string> KEY_WORDS(key_words, key_words+18);


string to_string(int a){
	stringstream ss;
	ss << a;
	string str = ss.str();
	return str;
}

void ValidateNumArgs(TreeNode* x_node, int num_args){

	TreeNode* temp = x_node;
	bool flag = true;
	for(int i=0;i<num_args && flag;i++){
		
		temp = temp->right;
		if(temp == NULL){
			flag = false;
		}
	}
	if(!flag || temp->atom_val_ != "NIL"){
		throw "Incorrect number of args to operation";
	}

}

bool is_numeric_literal(string atom_val){
	if(atom_val[0] != '-' && atom_val[0] != '+' && !isdigit(atom_val[0]) ){
		return false;
	}
	for(unsigned int i=1;i<atom_val.length();i++){
		if(!isdigit(atom_val[i])){
			return false;
		}
	}
	//cout<<"true!"<<endl;
	return true;
}

int get_numeric_val(string atom_val){
	return atoi(atom_val.c_str());
}

TreeNode* tree_car(TreeNode* node){
	//eval(node->left);
	//if(node->left != NULL)

	//TODO: VERY IMPORTANT: should i just throw an exception if node->left is null???
	if(node->left == NULL){
		throw "CAR accepts only list as input";
	}
	return node->left;
}
TreeNode* tree_cdr(TreeNode* node){
	//eval(node->left);
	//if(node->left != NULL)
	//TODO: VERY IMPORTANT: should i just throw an exception if node->right is null???
	if(node->right == NULL){
		throw "CDR accepts only list as input";
	}
	return node->right;
}

TreeNode* tree_atom(TreeNode* node){
	//TreeNode* evaluated_node = eval(node);
	TreeNode* node_to_return = new TreeNode();
	node_to_return->is_internal_node_ = false;
	//cout<<"node->is_internal_node_"<<node->is_internal_node_<<endl;
	//if evaluated node is an atom
	if (node->left == NULL && node->right == NULL){
		node_to_return->atom_val_ = "T";
	} 
	//if eval mode is not an atom
	else{
		node_to_return->atom_val_ = "NIL";
		node_to_return->is_list_node_ = true;
	}
	//DeleteTree(node);
	return node_to_return;
}

TreeNode* tree_cons(TreeNode* node1, TreeNode* node2){
	
	TreeNode* root = new TreeNode();
	root->left = node1;
	root->right = node2;
	//cout<<root->left->atom_val_<<" "<< root->left->is_list_node_<<" "<<root->right->atom_val_<<" "<<root->right->is_list_node_<<endl;

	if(tree_atom(root->right)->atom_val_ == "T" && root->right->atom_val_ != "NIL"){
		root->is_list_node_ = false;
	} else{
		root->is_list_node_ = true;
	}

	return root;
}


bool AreBothNodesAtoms(TreeNode* node1, TreeNode* node2){
	return (node1->left==NULL && node1->right == NULL && node2->left == NULL && node2->right == NULL);
}

TreeNode* tree_eq(TreeNode* node1, TreeNode* node2){
	
	//validate
	if(!AreBothNodesAtoms(node1, node2)){
		throw "Incorrect syntax: EQ needs 2 atoms";
	}

	TreeNode* node_to_return = new TreeNode();
	node_to_return->is_internal_node_ = false;
	
	

	string st1, st2;
	bool is_node1_numeric = is_numeric_literal(node1->atom_val_);
	bool is_node2_numeric = is_numeric_literal(node2->atom_val_);

	//if one is non-numeric and the other is
	if((is_node1_numeric && !is_node2_numeric) || (!is_node1_numeric && is_node2_numeric)){
		node_to_return->atom_val_ = "NIL";
	}
	
	//check if they are equal numericals
	if( is_node1_numeric && is_node2_numeric ){
		st1 = get_numeric_val(node1->atom_val_);
		st2 = get_numeric_val(node2->atom_val_);
		if(st1 == st2){
			node_to_return->atom_val_ = "T";
		} else{
			node_to_return->atom_val_ = "NIL";
		}
	}

	//check if they are equal non-numericals
	if( !is_node1_numeric && !is_node2_numeric){
		if(node1->atom_val_ == node2->atom_val_){
			node_to_return->atom_val_ = "T";
		} else{
			node_to_return->atom_val_ = "NIL";
		}
	}

	return node_to_return;	


	//return st1==st2;
}




TreeNode* tree_int(TreeNode* node){
	//TreeNode* evaluated_node = eval(node);
	TreeNode* node_to_return = new TreeNode();
	node_to_return->is_internal_node_ = false;
	//if node is an atom with numeric value
	if (!(node->is_internal_node_) && is_numeric_literal(node->atom_val_) ){
		node_to_return->atom_val_ = "T";
	} else{
		node_to_return->atom_val_ = "NIL";
		node_to_return->is_list_node_ = true;
	}
	//DeleteTree(node);
	return node_to_return;

}

TreeNode* tree_null(TreeNode* node){
	//TreeNode* evaluated_node = eval(node);
	TreeNode* node_to_return = new TreeNode();
	node_to_return->is_internal_node_ = false;
	//if node is an atom with numeric value
	if (tree_atom(node)->atom_val_ == "T" && node->atom_val_ == "NIL") {
		node_to_return->atom_val_ = "T";
	} else{
		node_to_return->atom_val_ = "NIL";
		node_to_return->is_list_node_ = true;
	}
	//DeleteTree(node);
	return node_to_return;
}

TreeNode* tree_arithmetic(TreeNode* node1, TreeNode* node2, ArithmeticOp arith_op){
	
	//validate
	if((!AreBothNodesAtoms(node1, node2)) || (!is_numeric_literal(node1->atom_val_)) || (!is_numeric_literal(node2->atom_val_))){
		throw "Incorrect syntax: All arithmetic operations need 2 numeric values as arguments";
	}

	int val1 = get_numeric_val(node1->atom_val_);
	int val2 = get_numeric_val(node2->atom_val_);

	TreeNode* node_to_return = new TreeNode();
	node_to_return->is_internal_node_ = false;

	switch(arith_op){
		case(kPlus):
			node_to_return->atom_val_ = to_string(val1+val2);break;
		case(kMinus):
			node_to_return->atom_val_ = to_string(val1-val2);break;
		case(kTimes):
			node_to_return->atom_val_ = to_string(val1*val2);break;
		case(kQuotient):	
			node_to_return->atom_val_ = to_string(val1/val2);break;
		case(kRemainder):	
			node_to_return->atom_val_ = to_string(val1%val2);break;
	
	}

	return node_to_return;
}

TreeNode* tree_lesser(TreeNode* node1, TreeNode* node2){
	
	//validate
	if((!AreBothNodesAtoms(node1, node2)) || (!is_numeric_literal(node1->atom_val_)) || (!is_numeric_literal(node2->atom_val_))){
		throw "Incorrect syntax: All relational operations need 2 numeric values as arguments";
	}

	int val1 = get_numeric_val(node1->atom_val_);
	int val2 = get_numeric_val(node2->atom_val_);

	TreeNode* node_to_return = new TreeNode();
	node_to_return->is_internal_node_ = false;

	if(val1 < val2){
		//TODO: move setting of atom_val_ to T to a helper function?
		node_to_return->atom_val_ = "T";
	} else{
		node_to_return->atom_val_ = "NIL";
	}

	return node_to_return;
}

void ValidateParamList(TreeNode* root){

	if(tree_atom(root)->atom_val_ == "T" ){
		if(root->atom_val_ != "NIL")
			throw "Parameter list can not be a non-NIL atom";
		else
			return;
	}

	TreeNode* temp = root;
	while(true){
		//if leaf node, must be NIL
		if(temp->right == NULL && temp->left == NULL){
			if(temp->atom_val_ != "NIL"){
				throw "Parameter list MUST be a list";
			}
			else
				return;
		}
		//each member must be an atom
		if(tree_atom(tree_car(temp))->atom_val_ != "T"){
			throw "Each member of parameter list MUST be an atom!";
		}
		temp = temp->right;
	}
}

void ValidateBody(TreeNode* node){
	TreeNode* temp = node;
	while(true){
		//if leaf node, must be NIL
		if(temp->right == NULL && temp->left == NULL){
			if(temp->atom_val_ != "NIL"){
				throw "Body MUST be a list";
			}
			else
				return;
		}
		temp = temp->right;
	}
}

void add_to_Dlist(TreeNode* root){
	//check to ensure that function name is an atom
	TreeNode* car_node = tree_car(root);
	if(tree_atom(car_node)->atom_val_ != "T"){
		throw "Function name must be an atom!";
	}
	//check to ensure that the atom_val of car_node is not a key word
	string function_name = car_node->atom_val_;
	if(KEY_WORDS.find(function_name) != KEY_WORDS.end()){
		throw "Function name can not be a key word or function!";
	}
	if(D_list.find(function_name) != D_list.end()){
		throw "Function name can not be an already defined function!";
	}

	ValidateNumArgs(root, 3);

	//check if paramlist is a list of atoms 
	ValidateParamList(tree_car(tree_cdr(root)));

	// check if body is a list
	ValidateBody(tree_car(tree_cdr(tree_cdr(root))));

	TreeNode* node = new TreeNode();
	node->left = tree_car(tree_cdr(root));
	node->right = tree_car(tree_cdr(tree_cdr(root)));

	D_list[function_name] = node;
}

TreeNode* getValFromAList(string formal_name_in_body, map< string, stack<TreeNode*> >& A_list){
	
	map< string, stack<TreeNode*> >::iterator a_list_it = A_list.find(formal_name_in_body);
	if(a_list_it == A_list.end()){
		throw "Function has an unidentified atom";
	} else{
		//TreeNode* node_to_return = new TreeNode();

		return a_list_it->second.top();
	}

}

TreeNode* Evaluator::eval(TreeNode* root, map< string, stack<TreeNode*> >& A_list){
	
	//check if root is an atom
	if(tree_atom(root)->atom_val_ == "T") {
		//CHECK whether atom_val_ is T, NIL or numeric expression. If not, throw exception
		string atom_val = root->atom_val_ ;
		
		if(atom_val == "T" || atom_val == "NIL" || is_numeric_literal(atom_val)) {
			TreeNode* node_to_return = new TreeNode();
			node_to_return->atom_val_ = atom_val;
			if(atom_val == "NIL"){
				node_to_return->is_list_node_ = true;
			}
			
			return node_to_return;
		} else if(A_list.find(atom_val) != A_list.end()){
			
			return getValFromAList(atom_val, A_list);
		
		} else {
			throw "Atom is an unidentified literal: Expected T, NIL, numeric value or a valid formal value";// + atom_val;
		}		
		//cout<<"returning root with atom_val_ "<<root->atom_val_<<endl;
		
	}

	if(root == NULL){
		//TODO: throw exception?
		throw "Error!";
		//cout<<"NULL ROOT!!"<<endl;
		//return root;
	}
	
	//TODO: Does this node have to be an atom?? I think so. If so, need to throw exception if it's not an atom
	TreeNode* car_node = tree_car(root);
	if(car_node == NULL || tree_atom(car_node)->atom_val_ != "T"){
		//TODO: throw exception????
		throw "Error!";
		
	}

	string car_val = car_node->atom_val_;
	
	if(car_val == "QUOTE"){
		
		//TODO: check whether root->right is valid for QUOTE
		//null check as well as contents of root->right->left?
		ValidateNumArgs(tree_cdr(root), 1);
		if(root->right == NULL || root->right->left == NULL || root->right->right == NULL){
			throw "QUOTE used wrongly\n";
		}
		TreeNode* node_to_return = root->right->left;

		//clean up - we don't need any of the elements of the subtree rooted at 'root' except for the subtree of this subtree which is rooted at root->right->left
		
		//TODO: uncomment!
		//root->right->left = NULL;
		//DeleteTree(root);
		
		/*root->left = NULL;
		delete car_node;

		TreeNode* temp = root->right->right;
		root->right->right = NULL;
		delete temp;

		root->right = NULL;
		delete root->right;
*/
		return node_to_return;
	}

	if(car_val== "COND"){

		//cout<<"entering evcon\n";
		return evcon(tree_cdr(root), A_list);
	}

	//DEFUN should be put here
	if(car_val== "DEFUN"){
		
		//add stuff to d-list
		add_to_Dlist(tree_cdr(root));
		return tree_car(tree_cdr(root));
		
	}

	//cout<<"Going to apply\n";
	return apply(tree_car(root), evlist(tree_cdr(root), A_list), A_list);

	return root;
}

TreeNode* getValFromDList(string function_name){
	
	map<string, TreeNode*>::iterator d_list_it = D_list.find(function_name);
	if(d_list_it == D_list.end()){
		throw "Invalid function called";
	} else{
		return d_list_it->second;
	}

}

int getSizeOfList(TreeNode* list_root){
	TreeNode* temp = list_root;

	int count=0;
	while(temp!=NULL){
		temp=temp->right;
		count++;
	}
	return count;

}

map< string, stack<TreeNode*> > add_pairs( TreeNode* formals_list, TreeNode* actuals_list, map< string, stack<TreeNode*> > A_list){
	int formals_list_size = getSizeOfList(formals_list);
	int actuals_list_size = getSizeOfList(actuals_list);

	if(formals_list_size != actuals_list_size){
		throw "Function called with incorrect number of parameters";
	}

	map< string, stack<TreeNode*> > new_A_list = A_list;

	TreeNode* temp_formals = formals_list;
	TreeNode* temp_actuals = actuals_list;

	while(tree_atom(temp_formals)->atom_val_ != "T" &&  tree_atom(temp_actuals)->atom_val_ != "T"){

		string formal_name = (tree_car(temp_formals))->atom_val_;
		map<string, stack<TreeNode*> >::iterator new_a_list_it = new_A_list.find(formal_name);


		stack<TreeNode*> stk;
		//A_list already has this formal name
		if(new_a_list_it != new_A_list.end()){
			stk = (new_a_list_it->second);
		} 
		stk.push(tree_car(temp_actuals));
		new_A_list[formal_name] = stk;
		
		temp_formals = temp_formals->right;
		temp_actuals = temp_actuals->right;
	}

	return new_A_list;

}

TreeNode* Evaluator::apply(TreeNode* f_node, TreeNode* x_node, map< string, stack<TreeNode*> > &A_list){
	if(tree_atom(f_node)->atom_val_ == "T"){

		string function_name = f_node->atom_val_;

		//TODO: Need to do alllll checks
		TreeNode* node_to_return = NULL;
		if(function_name == "CAR"){

			ValidateNumArgs(x_node, 1);

			node_to_return = tree_car(tree_car(x_node));
			
			//cleanup
			//x_node->left->left = NULL;
			//DeleteTree(x_node);

		} else if(function_name == "CDR"){
			ValidateNumArgs(x_node, 1);
			node_to_return = tree_cdr(tree_car(x_node));

		} else if(function_name == "CONS"){
			ValidateNumArgs(x_node, 2);
			node_to_return = tree_cons(tree_car(x_node), tree_car(tree_cdr(x_node)));


		} else if(function_name == "ATOM"){
			ValidateNumArgs(x_node, 1);
			node_to_return = tree_atom(tree_car(x_node));
			//delete x_node?
		} else if(function_name == "EQ"){
			ValidateNumArgs(x_node, 2);
			node_to_return = tree_eq(tree_car(x_node), tree_car(tree_cdr(x_node)) );
		} else if(function_name == "INT"){
			ValidateNumArgs(x_node, 1);
			node_to_return = tree_int(tree_car(x_node));
		} else if(function_name == "NULL"){
			ValidateNumArgs(x_node, 1);
			node_to_return = tree_null(tree_car(x_node));
		} else if(function_name == "PLUS"){
			ValidateNumArgs(x_node, 2);
			node_to_return = tree_arithmetic(tree_car(x_node), tree_car(tree_cdr(x_node)), kPlus);
		} else if(function_name == "MINUS"){
			ValidateNumArgs(x_node, 2);
			node_to_return = tree_arithmetic(tree_car(x_node), tree_car(tree_cdr(x_node)), kMinus );
		} else if(function_name == "TIMES"){
			ValidateNumArgs(x_node, 2);
			node_to_return = tree_arithmetic(tree_car(x_node), tree_car(tree_cdr(x_node)), kTimes);
		} else if(function_name == "QUOTIENT"){
			ValidateNumArgs(x_node, 2);
			node_to_return = tree_arithmetic(tree_car(x_node), tree_car(tree_cdr(x_node)), kQuotient);
		} else if(function_name == "REMAINDER"){
			ValidateNumArgs(x_node, 2);
			node_to_return = tree_arithmetic(tree_car(x_node), tree_car(tree_cdr(x_node)), kRemainder);
		} else if(function_name == "LESS"){
			ValidateNumArgs(x_node, 2);
			node_to_return = tree_lesser(tree_car(x_node), tree_car(tree_cdr(x_node)));
		} else if(function_name == "GREATER"){
			ValidateNumArgs(x_node, 2);
			node_to_return = tree_lesser( tree_car(tree_cdr(x_node)), tree_car(x_node));
		}

		else{
			map< string, stack<TreeNode*> > new_A_list = add_pairs( tree_car(getValFromDList(function_name)), x_node, A_list);
			return eval(tree_cdr(getValFromDList(function_name)), new_A_list);
			//cout<<function_name<<" ";
			//throw "Incorrect expression - Invalid function called";
		}
		

		return node_to_return;
	}
	throw "Incorrect expression - Expected an atom but didn't get one";
}

TreeNode* Evaluator::evlist(TreeNode* node, map< string, stack<TreeNode*> >& A_list){
	//cout<<"evlisttt\n";
	//TODO: change this text;

	if(node == NULL){
		throw "evlist needs non-null";
	}

	if(tree_atom(node)->atom_val_ == "T" && node->atom_val_ == "NIL"){
		TreeNode* node_to_return = new TreeNode();
		node_to_return->atom_val_ = "NIL";
		node_to_return->is_list_node_ = true;
		return node_to_return;
	}

	TreeNode* temp = eval(tree_car(node), A_list);
	return tree_cons(temp , evlist(tree_cdr(node), A_list));
}

TreeNode* Evaluator::evcon(TreeNode* node, map< string, stack<TreeNode*> >& A_list){
	
	//TODO: NEED to check whether number of parameters in each cond is 2 
	ValidateNumArgs(tree_car(node), 2);

	if(node == NULL){
		//TODO: throw exception
	}
	//TODO: check for NULL in tree_car
	TreeNode* caar_node = tree_car(tree_car(node));
	//cout<<"caar_node "<<caar_node->atom_val_<<endl;
	TreeNode* evaluated_caar = eval(caar_node, A_list);

	if(evaluated_caar == NULL){
		//TODO: throw exception?

	}
	if(evaluated_caar->atom_val_ == "T"){
		//cout<<"cadar"<<(tree_car(tree_cdr(tree_car(node))))->atom_val_<<endl;
		//cout<<"cadar"<<(tree_car(tree_cdr(tree_car(node))))->atom_val_<<endl;
		//TODO: throw exception?
		TreeNode* evaluated_cadar = eval(tree_car(tree_cdr(tree_car(node))), A_list );
		//node->left->right->left = evaluated_cadar;
		return evaluated_cadar;
	} else if (evaluated_caar->atom_val_ == "NIL"){
		return evcon(tree_cdr(node), A_list);

	} else{
		//TODO: throw exception?
		throw "Condition cannot be evaluated"; 
	}
}

