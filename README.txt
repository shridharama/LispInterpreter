Shridhar Ramachandran

**************
INSTRUCTIONS:
**************
Please enter the following commands in order to build and test the program.
make
./myinterpreter < inputfile > output file

**************
DESIGN INFORMATION:
**************
My code consists of 2 primary classes: Scanner and Parser. I also have 2 helper classes Token and TreeNode, which are used by the parser and scanner. The overall flow is - main calls Parser; Parser calls Scanner to get the next token and generates a tree on the fly. Once each root S-Expression is parsed entirely, the tree is pretty-printed. More details about individual classes below.

1. Token
This is a class that stores information about a token - token type (which is an enum), a boolean that mentions if it is an atom or not, as well as the token's string value. Objects of type Token are created by the scanner and returned to the Parser. Token has a derived class Atom, objects of which are created if the token being scanned is an atom. 

2. TreeNode
This is a class that stores information about nodes in the binary tree representation of the S-Exp. In addition to left and right node pointers, it also stores information about whether a node is a list or not and whether it is an internal node or not. If it's a leaf node, it stores the string value of the atom.

3. Scanner
This class contains the GetNextToken() method which processes character from stdin on the fly and returns a Token. If the token is an atom, it returns an Atom object (which is a derived class of Token). The scanner also validates whether an atom is in the correct format or not (i.e. if it starts with a numerical digit, it cannot contain characters). If the end of file is reached, it returns a null pointer.

4. Parser
This class contains the ParseStart() and ParseSexp() methods as mentioned in the project pdf provided. In addition, the ParseStart creates a root binary tree node. The remaining nodes are created on the fly as tokens are received from the scanner in ParseSexp(). All the required error checks are also done during ParseSexp().


5. Evaluator
This class contains all the methods that evaluate the tree created by the parser. The additional DEFUN logic is implemented very similar to the slides, with a few differences such as - my D-List is a map of function name (string) to a TreeNode and my A-List is a map of formals name (string) to a Stack of TreeNodes. My D-List is a global data structure so I don't pass it around.

**************
ACKNOWLEDGEMENTS:
**************
Thanks to Prof Neelam for hosting some valid and invalid test cases on his website.
