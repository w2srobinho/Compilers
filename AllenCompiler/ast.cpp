#include "ast.h"
#include "st.h"

using namespace AST;

extern ST::SymbolTable symtab;

/* Print methods */
void Value::printTree(){
  std::string strType ="";
  switch(type){
    case integer: strType = "inteiro "; break;
    case real: strType = "real "; break;
    case boolean: strType = "boolean "; break;
  }
  std::cout << "valor " << strType << " " << value;
}

void BinOp::printTree(){
  left->printTree();
  switch(op){
    case plus: std::cout << "soma "; break;
    case minus: std::cout << "subtração "; break;
    case division: std::cout << "divisão "; break;
    case times: std::cout << "multiplicação "; break;
    case assign: std::cout << "atribuição "; break;
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
  std::cout << id << " ";
}

void VarDeclaration::printTree(){
  std::string strType =  "undefined";
  switch (this->type) {
    case 0: strType = "int"; break;
    case 1: strType = "real"; break;
    case 2: strType = "boolean"; break;
  }
  std::cout << "Declaracao de variavel " << strType;
}
