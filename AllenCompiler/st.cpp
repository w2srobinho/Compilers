#include "st.h"

using namespace ST;

extern SymbolTable symtab;

AST::Node* SymbolTable::newVariable(std::string id, AST::Node* next) {
  std::cout << "new variable " << id << std::endl;

  if (checkId(id))
    yyerror("Variable redefinition! %s\n", id.c_str());
  else {
    Symbol entry(undefined, variable, "", false);
    undefined_types[id] = &entry;
    addSymbol(id,entry); //Adds variable to symbol table
  }
  return new AST::Variable(id, next); //Creates variable node anyway
}

void SymbolTable::updateSymbolTable(std::string strType) {
  std::cout << "update symbol table " << strType << std::endl;

  Type type = undefined;
  // c++ não tem suporte pra switch/case com string ):
  if (strType.compare("int:") == 0) type = integer;
  else if (strType.compare("real:") == 0) type = real;
  else if (strType.compare("bool:") == 0) type = boolean;
  std::cout << "Declaração de variável tipo " << strType << ": " << std::endl;
  for(auto const &symbol : undefined_types) {
    if (*begin(undefined_types)) {
      std::cout << "/* message */" << std::endl;
    }
    symbol->second->setType(type);
    std::cout << symbol->first <<" "<< std::endl;
  }
  undefined_types.clear();
}

AST::Node* SymbolTable::assignVariable(std::string id) {
  std::cout << "assign variable\n";

  if (!checkId(id)) {
    yyerror("Variable not defined yet! %s\n", id.c_str());
    // return NULL;
  }

  entryList[id].initialized = true;

  return new AST::Variable(id, NULL); //Creates variable node anyway
}

AST::Node* SymbolTable::useVariable(std::string id){
  std::cout << "use variable\n";

  if (!checkId(id))
    yyerror("Variable not defined yet! %s\n", id.c_str());
  if (!entryList[id].initialized)
    yyerror("Variable not initialized yet! %s\n", id.c_str());

  return new AST::Variable(id, NULL); //Creates variable node anyway
}
