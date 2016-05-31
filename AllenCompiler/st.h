/*Symbol Table definition*/
#pragma once

#include <map>
#include <iostream>
#include <string>
#include "ast.h"

extern void yyerror(const char* s, ...);

namespace ST {

class Symbol;

  enum Kind { variable };

  typedef std::map<std::string, Symbol> SymbolList; //Set of Symbols

  class Symbol {
    public:
      AST::Type type;         /*[Return] type of Symbol: integer, real, bool.*/
      Kind kind;         /*Kind of symbol: variable, function, etc.*/
      bool initialized;  /*Defines if symbol has been initialized or not.*/

      Symbol(AST::Type type, Kind kind, std::string value, bool initialized) :
        type(type),
        kind(kind),
        initialized(initialized) {  }

      Symbol() {
        type = AST::undefined;
        kind = variable;
        initialized = false;
      }

      void setType(AST::Type type) {
        this->type = type;
      }
  };

  class SymbolTable {
    private:
      std::map<std::string, Symbol*> undefined_types;

    public:
      SymbolList entryList;
      SymbolTable() {}

      /*checkId returns true if the variable has been defined and false if it does not exist*/
      bool checkId(std::string id)
      {
        return entryList.find(id) != entryList.end();
      }

      void addSymbol(std::string id, Symbol newsymbol)
      {
        entryList[id] = newsymbol;
      }

      void updateSymbolTable(std::string strType);

      AST::Node* newVariable(std::string id);
      AST::Node* assignVariable(std::string id);
      AST::Node* useVariable(std::string id);
  };
}
