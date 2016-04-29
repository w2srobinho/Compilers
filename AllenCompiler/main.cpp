#include <iostream>
#include "ast.h"
#include "st.h"

extern AST::Block* programRoot; //set on Bison file
extern int yyparse();

int main(int argc, char **argv)
{
    yyparse();                  //parses whole data
    std::cout << "##Printing the lines" << std::endl;
    programRoot->printTree();   //prints the ASTs
    // std::cout << "##Computing the lines" << std::endl;
    // programRoot->computeTree(); //computes the ASTs
    return 0;
}
