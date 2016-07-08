/*Abstract Syntax Tree definition*/
#pragma once

#include <iostream>
#include <vector>

extern void yyerror(const char *s, ...);

namespace AST {

  //Binary operations
  enum Operation { plus, minus, times, division, andop, orop, assign };
  enum Type { unknown, integer, real, boolean  }; /*Possible types*/

  class Node;

  typedef std::vector<Node*> NodeList; //List of ASTs

  class Node {
    public:
      Type type = unknown;
      virtual ~Node() {  }
      virtual void printTree() {  }
      Node() {  }
      Node(Type type) :
        type(type) {  }
  };

  class Block : public Node {
    public:
      NodeList lines;
      Block() {}
      void printTree();
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
        void printTree();
  };

  class VarDeclaration : public Node {
    public:
      NodeList variables;
      VarDeclaration(Type type) :
        Node(type) {  }
        void printTree();
  };

  class Variable : public Node {
    public:
      std::string id;
      Variable(std::string id, Type type) :
        id(id),
        Node(type) {  }
        void printTree();
  };

  class Value : public Node {
    public:
      std::string value;
      Value(std::string value, Type type) :
        value(value),
        Node(type) {  }
      void printTree();
  };

  class If : public Node {
    public:
      Node *expression;
      Node *them;
      Node *elsepart;
      If(Node *expression, Node *them, Node *elsepart) :
        expression(expression),
        them(them),
        elsepart(elsepart) { }
      void printTree();
  };

  class FuncDeclaration : public Node {
    public:
      std::string id;
      NodeList *args;
      Node *block;
      FuncDeclaration(std::string id, NodeList *args, Node *block) :
        id(id),
        args(args),
        block(block) { }
      void printTree();
  };

  class FunCall : public Node {
    public:
      std::string id;
      NodeList *params;
      FunCall(std::string id, NodeList *params) :
        id(id),
        params(params) { }
      void printTree();
  }
}
