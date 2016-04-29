/*Abstract Syntax Tree definition*/
#pragma once

#include <iostream>
#include <vector>

extern void yyerror(const char *s, ...);

namespace AST {

  //Binary operations
  enum Operation { plus, times, assign };

  class Node;

  typedef std::vector<Node*> NodeList; //List of ASTs

  class Node {
  public:
    virtual ~Node() {}
    virtual void printTree(){}
    virtual std::string computeTree(){return "";}
  };

  class Value : public Node {
  public:
    char* value;
    Value(char* value) : value(value) {  }
    void printTree();
    // std::string computeTree();
  };

  class BinOp : public Node {
  public:
    Operation op;
    Node *left;
    Node *right;
    BinOp(Node *left, Operation op, Node *right) :
    left(left), right(right), op(op) { }
    void printTree();
    // std::string computeTree();
  };

  class Block : public Node {
  public:
    NodeList lines;
    Block() { }
    void printTree();
    // std::string computeTree();
  };

  class Variable : public Node {
  public:
    std::string id;
    Node *next;
    Variable(std::string id, Node *next) :
      id(id),
      next(next) { }
    void printTree();
    std::string computeTree();
  };
}
