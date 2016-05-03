#include "st.h"

using namespace ST;

extern SymbolTable symtab;

AST::Node* SymbolTable::newVariable(std::string id, AST::Node* next) {
  std::cout << "new variable " << id << std::endl;

  if (checkId(id))
    yyerror("Variable redefinition! %s\n", id.c_str());
  else {
    Symbol entry(undefined, variable, "", false);

    addSymbol(id,entry); //Adds variable to symbol table
    undefined_types[id] = &entryList[id];
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
  std::cout << "Declaração de variável tipo " << strType << ": ";

  for(auto undefined_symbol : undefined_types) {
    if (begin(undefined_types)->first == undefined_symbol.first) {
      std::cout << undefined_symbol.first;
    } else {
      std::cout << ", " << undefined_symbol.first;
    }
    undefined_symbol.second->setType(type);
  }
  std::cout << ";\n";
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
