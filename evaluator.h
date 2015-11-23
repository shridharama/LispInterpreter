#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "common.h"
#include <stack>
#include <map>

class Evaluator{

public:
  	Evaluator() { }


  	
	TreeNode* eval(TreeNode* root, map< string, stack<TreeNode*> >& A_list);

	TreeNode* evcon(TreeNode* node, map< string, stack<TreeNode*> >& A_list);
  	
  	TreeNode* evlist(TreeNode* node, map< string, stack<TreeNode*> >& A_list);

  	TreeNode* apply(TreeNode* f_node, TreeNode* x_node, map< string, stack<TreeNode*> >& A_list);
};


#endif //EVALUATOR_H