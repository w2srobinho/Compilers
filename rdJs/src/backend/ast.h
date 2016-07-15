/*Abstract Syntax Tree definition*/
#pragma once

#include "llvm/Analysis/Passes.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/PassManager.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Transforms/Scalar.h"

#include <cctype>
#include <cstdio>
#include <map>
#include <string>
#include <vector>
#include <iostream>

extern void yyerror(const char *s, ...);

namespace AST {

  //Binary operations
  enum Operation { plus, minus, times, division, andop, orop, assign, comp };
  enum Type { unknown, integer, real, boolean  }; /*Possible types*/
  enum Usage { declare, load, store }; /*Possible types*/

  class Node;

  typedef std::vector<Node*> NodeList; //List of ASTs

  class Node {
    public:
      Type type = unknown;
      virtual ~Node() {  }
      Node() {  }
      Node(Type type) :
        type(type) {  }
      virtual llvm::Value *Codegen() = 0;
  };

  class Main : public Node {
  public:
    Node *block;
    Main(Node *block) :
      block(block) {}
    virtual llvm::Value *Codegen();
  };

  class Block : public Node {
    public:
      NodeList lines;
      Block() {}
      virtual llvm::Value *Codegen();
  };

  class BinOp : public Node {
    public:
      Node *left;
      Operation op;
      Node *right;
      BinOp(Node *left, Operation op, Node *right) :
        left(left),
        right(right),
        op(op) {
          // type = Types::binType(left->type, right->type, op);
        }
        virtual llvm::Value *Codegen();
  };

  class VarDeclaration : public Node {
    public:
      NodeList variables;
      VarDeclaration(Type type) :
        Node(type) {  }
      virtual llvm::Value *Codegen();
  };

  class Variable : public Node {
  public:
      std::string id;
      Usage usage;
      Variable(std::string id, Usage usage) :
        id(id),
        usage(usage) {  }
      virtual llvm::Value *Codegen();
  };

  class Number : public Node {
      std::string number;
    public:
      Number(std::string number, Type type) :
        number(number),
        Node(type) {  }
      virtual llvm::Value *Codegen();
  };

  class If : public Node {
      Node *condition;
      Node *then;
      Node *elsepart;
    public:
      If(Node *condition, Node *then, Node *elsepart) :
        condition(condition),
        then(then),
        elsepart(elsepart) { }
      virtual llvm::Value *Codegen();
  };

  class FuncDeclaration : public Node {
      std::string id;
      NodeList *args;
      Node *block;
    public:
      FuncDeclaration(std::string id, NodeList *args, Node *block) :
        id(id),
        args(args),
        block(block) { }
      virtual llvm::Value *Codegen();
  };

  class FunCall : public Node {
      std::string id;
      NodeList *params;
    public:
      FunCall(std::string id, NodeList *params) :
        id(id),
        params(params) { }
      virtual llvm::Value *Codegen();
  };

  class For : public Node {
      Node *interactions;
      Node *adder;
      Node *loop;
    public:
      For(Node *i, Node *a, Node *l) :
        interactions(i),
        adder(a),
        loop(l) {}
      virtual llvm::Value *Codegen();
  };

  static Node *Error(const char *Str) { fprintf(stderr, "Error: %s\n", Str);return 0;}
}
