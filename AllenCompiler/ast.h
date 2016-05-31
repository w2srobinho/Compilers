/*Abstract Syntax Tree definition*/
#pragma once

#include <iostream>
#include <vector>

extern void yyerror(const char *s, ...);

namespace AST {
  //Operations
  enum Operation {
    plus,
    minus,
    division,
    times,
    assign,
    equals,
    differ,
    less,
    greater,
    greaterOrEqual,
    lessOrEqual,
    par,
    unMinus,
    not_op,
    and_op,
    or_op,
  };
  const std::vector<std::string> opName = {
    "soma",
    "subtração",
    "divisão",
    "multiplicação",
    "atribuição",
    "igual",
    "diferente",
    "menor",
    "maior",
    "maior ou igual",
    "menor ou igual",
    "parenteses",
    "menos unário",
    "não",
    "e",
    "ou"
  };
  enum Type { undefined, integer, real, boolean };
  const std::vector<std::string> typeName = {
    "indefinido",
    "inteiro",
    "real",
    "booleano"
  };
  class Node;

  typedef std::vector<Node*> NodeList; //List of ASTs

  class Node {
  public:
    virtual ~Node() {}
    virtual void printTree(){}
  };

  class Value : public Node {
  public:
    std::string value;
    Type type;
    Value(std::string value, Type type) :
      value(value),
      type(type) { }
    void printTree();
  };


  class Block : public Node {
  public:
    NodeList lines;
    Block() { }
    void printTree();
  };

  class BinOp : public Node {
  public:
    Operation op;
    Node *left;
    Node *right;
    BinOp(Node *left, Operation op, Node *right) :
      left(left),
      right(right),
      op(op) { }
    void printTree();
  };

  class UnOp : public Node {
  public:
    Operation op;
    Node *next;
    UnOp(Operation op, Node *next) :
      op(op),
      next(next) { }
    void printTree();
  };

  class VarDeclaration : public Node {
  public:
    Type type;
    Node* next;
    NodeList nodes;
    void setNext(Node* node) {
      next = node;
    }
    VarDeclaration(std::string strType, NodeList *varlist) :
    nodes(*varlist)
    {
      if(strType == "int") type = integer;
      else if(strType == "real") type = real;
      else if(strType == "bool") type = boolean;
      else type = undefined;
   }
    void printTree();
  };

  class Variable : public Node {
  public:
    std::string id;
    Type type;
    Variable(std::string id, Type type) :
    id(id),
    type(type) { }
    void printTree();
  };

  class ArrayDeclaration : public Node {
  public:
    Type type;
    std::string size;
    NodeList nodes;
    ArrayDeclaration(std::string strType, std::string size) :
    size(size)
    {
      if(strType == "int") type = integer;
      else if(strType == "real") type = real;
      else if(strType == "bool") type = boolean;
      else type = undefined;
    }
    void setVarList(NodeList *varlist) {
      nodes = *varlist;
    }
    void printTree();
  };

  class Array : public Node {
  public:
    std::string id;
    Type type;
    Node* index;
    Array(std::string id, Type type) :
      id(id),
      type(type) { }
    void printTree();
    void setPosition(Node* node) {
      index = node;
    }
  };
}
