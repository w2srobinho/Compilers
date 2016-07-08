#include "ast.h"

using namespace AST;

//extern ST::SymbolTable symtab;

/* Print methods */
void BinOp::printTree(){
    switch(op){
      case Operation::assign:
          /*Atribuicao de valor para variavel TIPO FEMININO nome:*/
          std::cout << "Atribuicao de valor para ";
          left->printTree();
          std::cout << ": ";
          right->printTree();
          break;
      default:
          std::cout << "(";
          left->printTree();
          std::cout << " op ";
          right->printTree();
          std::cout << ")";
          break;
    }
}

void Block::printTree(){
    for (Node* line: lines) {
        line->printTree();
        std::cout << std::endl;
    }
}

void Variable::printTree(){
    std::cout << "variavel " << id;
}

void VarDeclaration::printTree(){
    std::cout << "Declaracao de variavel: ";
    for (auto var = variables.begin(); var != variables.end(); var++) {
        std::cout << dynamic_cast<Variable *>(*var)->id;
        if(next(var) != variables.end()) std::cout << ", ";
    }
}

void Value::printTree(){
    std::cout << "valor " << value;
}

void If::printTree() {
  std::cout << "if " << std::endl;
  expression->printTree();
  std::cout << "them " << std::endl;
  them->printTree();
  if(elsepart != nullptr) {
    std::cout << "else " << std::endl;
    elsepart->printTree();
  }
}

void FuncDeclaration::printTree() {
  std::cout << "func "<< id << "(";
  for (auto arg : *args) {
    std::cout << dynamic_cast<Variable *>(arg)->id << " ";
  }
  std::cout << ") {" << std::endl;
  block->printTree();
  std::cout << "}" << std::endl;
}
