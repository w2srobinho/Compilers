#include "ast.h"
#include "st.h"

using namespace AST;

extern ST::SymbolTable symtab;

/* Print methods */
void Value::printTree(){
  std::string strType ="";
  switch(type){
    case integer: strType = " inteiro "; break;
    case real: strType = " real "; break;
    case boolean: strType = " boolean "; break;
  }
  std::cout << " valor " << strType << " " << value;
}

void BinOp::printTree(){
    left->printTree();
    switch(op){
      case plus: std::cout << " soma "; break;
      case minus: std::cout << " subtração "; break;
      case division: std::cout << " divisão "; break;
      case times: std::cout << " multiplicação "; break;
      case assign: std::cout << " atribuição "; break;
    }
    right->printTree();
    return;
}

void Block::printTree(){
    for (Node* line: lines) {
        line->printTree();
        std::cout << std::endl;
    }
}

void Variable::printTree(){
    if (next != NULL){
      next->printTree();
      std::cout << ", ";
    }
    std::cout << id;
}

/* Compute methods */
/*
Métodos para interpretação de código, provavelmente nunca serão usados
int Integer::computeTree(){
    return value;
}

int BinOp::computeTree(){
    int value, lvalue, rvalue;
    lvalue = left->computeTree();
    rvalue = right->computeTree();
    switch(op){
        case plus: value = lvalue + rvalue; break;
        case times: value = lvalue * rvalue; break;
        case assign:
            Variable* leftvar = dynamic_cast<Variable*>(left);
            symtab.entryList[leftvar->id].value = rvalue;
            value = rvalue;
    }
    return value;
}

int Block::computeTree(){
    int value;
    for (Node* line: lines) {
        value = line->computeTree();
         std::cout << "Computed " << value << std::endl;
    }
    return 0;
}

std::string Variable::computeTree(){
    return symtab.entryList[id].value;
}
*/
