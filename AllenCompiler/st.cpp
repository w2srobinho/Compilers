#include "st.h"

using namespace ST;

extern SymbolTable symtab;

AST::Node* SymbolTable::newVariable(std::string id, AST::Node* next) {
  if (checkId(id))
    yyerror("Variable redefinition! %s\n", id.c_str());
  else {
    Symbol entry(AST::undefined, variable, "", false);
    addSymbol(id,entry); //Adds variable to symbol table
    undefined_types[id] = &entryList[id];
  }
  return new AST::Variable(id, next, AST::undefined); //Creates variable node anyway
}

void SymbolTable::updateSymbolTable(std::string strType) {
  AST::Type type = AST::undefined;
  // c++ não tem suporte pra switch/case com string ):
  if (strType.compare("int") == 0) type = AST::integer;
  else if (strType.compare("real") == 0) type = AST::real;
  else if (strType.compare("bool") == 0) type = AST::boolean;

  for(auto undefined_symbol : undefined_types) {
    undefined_symbol.second->setType(type);
  }
  undefined_types.clear();
}

AST::Node* SymbolTable::assignVariable(std::string id) {
  AST::Type type = AST::undefined;
  if (!checkId(id)) {
    yyerror("Variable not defined yet! %s\n", id.c_str());
  }
  else {
    type = entryList[id].type;
  }
  entryList[id].initialized = true;
  return new AST::Variable(id, NULL, type); //Creates variable node anyway
}

AST::Node* SymbolTable::useVariable(std::string id) {
  AST::Type type = AST::undefined;
  if (!checkId(id))
    yyerror("Variable not defined yet! %s\n", id.c_str());
  else {
    type = entryList[id].type;
  }
  if (!entryList[id].initialized)
    yyerror("Variable not initialized yet! %s\n", id.c_str());

  return new AST::Variable(id, NULL, type); //Creates variable node anyway
}
