#include "ast.h"
#include "st.h"

using namespace AST;

extern ST::SymbolTable symtab;

/* Print methods */
void Value::printTree(){
  std::cout << "valor " << typeName[type] << " " << value;
}

void BinOp::printTree(){
  switch(op){
    case assign:
      std::cout << "Atribuição de valor para ";
      left->printTree();
      std::cout << ": ";
      right->printTree();
      break;
    default:
      std::cout << "(";
      left->printTree();
      std::cout << " (" << opName[op] << ") ";
      right->printTree();
      std::cout << ")";
      break;
    }
  return;
}

void UnOp::printTree(){
  std::cout << "(";
  switch(op){
    case par:
      std::cout << "(abre parenteses) ";
      next->printTree();
      std::cout << " (fecha parenteses)";
      break;
    default:
      std::cout << "(" << opName[op];
      std::cout << ") ";
      next->printTree();
      break;
  }
  std::cout << ")";
}

void Block::printTree(){
    for (Node* line: lines) {
        line->printTree();
        std::cout << std::endl;
    }
}

void Variable::printTree(){
  std::cout << "variável " << typeName[type] << " " << id;
}

/*TODO Alterar a impressão de variável e da varDeclaration para que não fique com ": ," no final da impressão*/
void VarDeclaration::printTree(){
  std::cout << "Declaração de variável " << typeName[ type] << ": ";
  for (auto var = nodes.begin(); var != nodes.end(); var++) {
        std::cout << dynamic_cast<Variable *>(*var)->id;
        if(std::next(var) != nodes.end()) std::cout << ", ";
    }
}

void ArrayDeclaration::printTree(){
  std::cout << "Declaração de arranjo "<< typeName[type] << " de tamanho " << size << ": ";
}
