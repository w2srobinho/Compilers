/*Symbol Table definition*/
#pragma once

#include <map>
//#include "ast.h"
#include <iostream>

extern void yyerror(const char* s, ...);

namespace ST {

class Symbol;

enum Kind { variable, function };

typedef std::map<std::string,Symbol> SymbolList; //Set of Symbols

  class Symbol {
    public:
      Kind kind;            /*Kind of symbol: variable, function */
      bool initialized;     /*Defines if symbol has been initialized or not.*/
      int params;
      Symbol(Kind kind, bool initialized) :
          kind(kind), initialized(initialized) {  }
      Symbol() {kind = variable; initialized = false;}
  };

  class SymbolTable {
    public:
      SymbolList entryList;
      SymbolTable() {}

      void newVariable(std::string id, Kind kind, bool initialized);
      void newFunction(std::string id, Kind kind, int argsNumber);
      void assignVariable(std::string id);
      void useVariable(std::string id);
      void useFunction(std::string id, int paramNumber);
    private:
      /*checkId returns true if the variable has been defined and false if it does not exist*/
      bool checkId(std::string id) { return entryList.find(id) != entryList.end(); }
      void addSymbol(std::string id, Symbol newsymbol) { entryList[id] = newsymbol; }
  };
}
