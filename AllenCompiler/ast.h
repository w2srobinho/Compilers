/*Abstract Syntax Tree definition*/
#pragma once

#include <iostream>
#include <vector>

extern void yyerror(const char *s, ...);

namespace AST {

  //Binary operations
  enum Operation { plus, minus, division, times, assign };
  enum Type { integer, real, boolean };

  class Node;

  typedef std::vector<Node*> NodeList; //List of ASTs

  class Node {
  public:
    // virtual void setNext(){ }
    virtual ~Node() {}
    virtual void printTree(){}
  };

  class Value : public Node {
  public:
    char* value;
    Type type;
    Value(char* value, Type type) : value(value), type(type) {  }
    void printTree();
  };

  class BinOp : public Node {
  public:
    Operation op;
    Node *left;
    Node *right;
    BinOp(Node *left, Operation op, Node *right) : left(left), right(right), op(op) { }
    void printTree();
  };

  class VarDeclaration : public Node {
  public:
    Type type;
    VarDeclaration(std::string strType) {
      if(strType == "int") type = integer;
      else if(strType == "real") type = real;
      else if(strType == "bool") type = boolean;
   }
    void printTree();
  };

  class Block : public Node {
  public:
    NodeList lines;
    Block() { }
    void printTree();
  };

  class Variable : public Node {
  public:
    std::string id;
    Node *next;
    void setNext(Node* node) {
      if(next)
        dynamic_cast<AST::Variable*>(next)->setNext(node);
      else
        next = node;
    }
    Variable(std::string id, Node *next) :
      id(id),
      next(next) { }
    void printTree();
  };
}
