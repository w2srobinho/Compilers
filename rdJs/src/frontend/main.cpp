#include <iostream>
#include "ast.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"

extern AST::Main *programRoot; //set on Bison file
extern int yyparse();
extern int yydebug;
extern llvm::Module *TheModule;

int main(int argc, char **argv)
{
  //yydebug = 1;              //remove comment to have verbose debug messages
  yyparse();                  //parses whole data

  //programRoot->Codegen();   //Generate code
  //TheModule->dump(); // Print out all of the generated code.

  std::cout << "Finnished!" << std::endl;
  return 0;
}
