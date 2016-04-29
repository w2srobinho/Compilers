/*Symbol Table definition*/
#pragma once

#include <map>
#include <iostream>
#include <string>
#include "ast.h"

extern void yyerror(const char* s, ...);

namespace ST {

class Symbol;

  enum Type {
    undefined,
    integer,
    real,
    boolean
  };
  enum Kind { variable };

  typedef std::map<std::string, Symbol> SymbolList; //Set of Symbols

  class Symbol {
    public:
      Type type;         /*[Return] type of Symbol: integer, real, bool.*/
      Kind kind;         /*Kind of symbol: variable, function, etc.*/
      std::string value; /*Space to store a value while we are doing interpretation.*/
      bool initialized;  /*Defines if symbol has been initialized or not.*/

      Symbol(Type type, Kind kind, std::string value, bool initialized) :
        type(type),
        kind(kind),
        value(value),
        initialized(initialized) {  }

      Symbol() {
        type = undefined;
        kind = variable;
        value = "";
        initialized = false;
      }

      void setType(Type type) {
        this->type = type;
      }
  };

  class SymbolTable {
    private:
      std::vector<Symbol *> undefined_types;

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

      AST::Node* newVariable(std::string id, AST::Node* next);
      AST::Node* assignVariable(std::string id);
      /* TODO verificar a necessidade do método */
      AST::Node* useVariable(std::string id);
  };
}
