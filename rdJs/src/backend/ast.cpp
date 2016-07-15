#include "ast.h"

using namespace AST;
using namespace llvm;
//extern ST::SymbolTable symtab;

#include <iostream>
#include <string>
#include <map>

llvm::LLVMContext &Context = llvm::getGlobalContext();
static std::map<std::string, AllocaInst*> NamedValues;
static llvm::IRBuilder<> Builder(Context);
llvm::Module *TheModule = new llvm::Module("Code generating", Context);

llvm::Value *ErrorV(const char *str) { Error(str); return 0;}

llvm::Value *Main::Codegen() {
  llvm::Type* intType = llvm::Type::getInt64Ty(Context);
  llvm::FunctionType* typeOfMain = llvm::FunctionType::get(llvm::Type::getDoubleTy(Context),false);
  llvm::Function* ourMain = llvm::Function::Create(typeOfMain,llvm::Function::ExternalLinkage, "main", TheModule);
  llvm::BasicBlock *mainBB = llvm::BasicBlock::Create(Context, "entry", ourMain);
  Builder.SetInsertPoint(mainBB);
  block->Codegen();
}

llvm::Value *Block::Codegen(){
  for (Node* line: lines) {
    line->Codegen();
  }
}

llvm::Value *BinOp::Codegen() {
  llvm::Value *r = right->Codegen();
  llvm::Value *l = left->Codegen();
  if (l == 0 || r == 0) return 0;

  switch(op) {
    case Operation::plus : return Builder.CreateAdd(l, r, "addtmp");
    case Operation::minus : return Builder.CreateSub(l, r, "subtmp");
    case Operation::times : return Builder.CreateMul(l, r, "multmp");
    case Operation::andop : return Builder.CreateAnd(l, r, "andtmp");
    case Operation::orop : return Builder.CreateOr(l, r, "ortmp");
    case Operation::assign : return Builder.CreateStore(r, l);
    case Operation::comp :
      l = Builder.CreateFCmpULT(l, r, "cmptmp");
      // Convert bool 0/1 to double 0.0 or 1.0
      return Builder.CreateUIToFP(l, llvm::Type::getDoubleTy(Context),
                                "booltmp");
    default : return ErrorV("invalid binary operator");
  }
}
llvm::Value *VarDeclaration::Codegen(){
  for (auto var = variables.begin(); var != variables.end(); var++) {
      NamedValues[dynamic_cast<Variable *>(*var)->id] = Builder.CreateAlloca(llvm::Type::getDoubleTy(Context),
      0,
      dynamic_cast<Variable *>(*var)->id.c_str());
  }
  return 0;
}
llvm::Value *Variable::Codegen(){
  Value *V = NamedValues[id];
  if (V == 0) return ErrorV("Unknown variable name");
  if(usage == Usage::load)
    return Builder.CreateLoad(V, id.c_str());
  return V;
}

llvm::Value *Number::Codegen() {
  if(number == "true") return llvm::ConstantFP::get(Context, APFloat(1.0));
  if(number == "false") return llvm::ConstantFP::get(Context, APFloat(0.0));
  return llvm::ConstantFP::get(Context, APFloat(stof(number)));
}

llvm::Value *If::Codegen(){
  Value *CondV = condition->Codegen();
  if (CondV == 0) return 0;

  // Convert condition to a bool by comparing equal to 0.0.
  CondV = Builder.CreateFCmpONE(CondV,
                              llvm::ConstantFP::get(Context, APFloat(0.0)),
                                "ifcond");

  Function *TheFunction = Builder.GetInsertBlock()->getParent();

  // Create blocks for the then and else cases.  Insert the 'then' block at the
  // end of the function.
  BasicBlock *ThenBB = llvm::BasicBlock::Create(Context, "then", TheFunction);
  BasicBlock *ElseBB = llvm::BasicBlock::Create(Context, "else");
  BasicBlock *MergeBB = llvm::BasicBlock::Create(Context, "ifcont");

  Builder.CreateCondBr(CondV, ThenBB, ElseBB);
  // Emit then value.
  Builder.SetInsertPoint(ThenBB);

  Value *ThenV = then->Codegen();
  if (ThenV == 0) return 0;

  Builder.CreateBr(MergeBB);
  // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
  ThenBB = Builder.GetInsertBlock();

  // Emit else block.
  TheFunction->getBasicBlockList().push_back(ElseBB);
  Builder.SetInsertPoint(ElseBB);

  Value *ElseV = elsepart->Codegen();
  if (ElseV == 0) return 0;

  Builder.CreateBr(MergeBB);
  // Codegen of 'Else' can change the current block, update ElseBB for the PHI.
  ElseBB = Builder.GetInsertBlock();

  // Emit merge block.
  TheFunction->getBasicBlockList().push_back(MergeBB);
  Builder.SetInsertPoint(MergeBB);
  llvm::PHINode *PN = Builder.CreatePHI(llvm::Type::getDoubleTy(Context), 2,
                                  "iftmp");

  PN->addIncoming(ThenV, ThenBB);
  PN->addIncoming(ElseV, ElseBB);
  return PN;
}

llvm::Value *FuncDeclaration::Codegen(){

}
llvm::Value *FunCall::Codegen(){

}
llvm::Value *For::Codegen(){

}
// /* Print methods */
// void BinOp::printTree(){
//     switch(op){
//       case Operation::assign:
//           /*Atribuicao de valor para variavel TIPO FEMININO nome:*/
//           std::cout << "Atribuicao de valor para ";
//           left->printTree();
//           std::cout << ": ";
//           right->printTree();
//           break;
//       default:
//           std::cout << "(";
//           left->printTree();
//           std::cout << " op ";
//           right->printTree();
//           std::cout << ")";
//           break;
//     }
// }
//
// void Block::printTree(){
//     for (Node* line: lines) {
//         line->printTree();
//         std::cout << std::endl;
//     }
// }
//
// void Variable::printTree(){
//     std::cout << "variavel " << id;
// }
//
// void VarDeclaration::printTree(){
//     std::cout << "Declaracao de variavel: ";
//     for (auto var = variables.begin(); var != variables.end(); var++) {
//         std::cout << dynamic_cast<Variable *>(*var)->id;
//         if(next(var) != variables.end()) std::cout << ", ";
//     }
// }
//
// void Value::printTree(){
//     std::cout << "valor " << value;
// }
//
// void If::printTree() {
//   std::cout << "if ";
//   expression->printTree();
//   std::cout << "them " << std::endl;
//   them->printTree();
//   if(elsepart != nullptr) {
//     std::cout << "else " << std::endl;
//     elsepart->printTree();
//   }
// }
//
// void FuncDeclaration::printTree() {
//   std::cout << "func "<< id << "(";
//   for (auto arg : *args) {
//     std::cout << dynamic_cast<Variable *>(arg)->id << " ";
//   }
//   std::cout << ") {" << std::endl;
//   block->printTree();
//   std::cout << "}" << std::endl;
// }
//
// void FunCall::printTree() {
//   std::cout << id << "(";
//   for (auto param : *params) {
//     param->printTree();
//   }
//   std::cout << ");" << std::endl;
// }
//
// void For::printTree() {
//   std::cout << "for (";
//   interactions->printTree();
//   std::cout << "; ";
//   adder->printTree();
//   std::cout << ") {" << std::endl;
//   loop->printTree();
//   std::cout << "}" << std::endl;
//
// }
