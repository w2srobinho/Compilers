#include <iostream>

extern const char *programRoot; //set on Bison file
extern int yyparse();
extern int yydebug;

int main(int argc, char **argv)
{
    //yydebug = 1;              //remove comment to have verbose debug messages
    yyparse();                  //parses whole data
    //programRoot->printTree();   //print the ASTs
    std::cout << programRoot << std::endl;
    return 0;
}
