

#ifndef PARSE_TREE_NODES_H
#define PARSE_TREE_NODES_H

#include <iostream>
#include <vector>
#include <string>
#include "lexer.h"

using namespace std;

extern bool printDelete;      // shall we print deleting the tree?

// ---------------------------------------------------------------------
// Forward declaration of node types
class ExprNode; 
class TermNode;
class FactorNode;
class IdNode;
class IntLitNode;
class NestedExprNode;
class SimpleExprNode;
class NotNode;
class MinusNode;
class FloatLitNode;
class BlockNode;
class CompoundNode;
class StatementNode;
class AssignmentNode;
class ExprNode;


// ---------------------------------------------------------------------
// <program> -> <statement>*
class ProgramNode {
public:
  int level;
  BlockNode* block = nullptr;

  ProgramNode(int _level);
  ~ProgramNode();
};
ostream& operator<<(ostream&, ProgramNode&);
// ---------------------------------------------------------------------
// <block> -> { <statement>* }
class BlockNode {
public:
  int level;
  CompoundNode* compound = nullptr;

  BlockNode(int _level);
  ~BlockNode();
};
ostream& operator<<(ostream&, BlockNode&);

// ---------------------------------------------------------------------
// <statement> -> <expr> ;
class StatementNode {
public:
  int level = 0;          // recursion level of this node
 
  virtual ~StatementNode();
  virtual void printTo(ostream& os) = 0;
};
ostream& operator<<(ostream&, StatementNode&);

// <assignment> -> ID = <expr> ;
class AssignmentNode : public StatementNode {
public:
  string id;               // variable name
  ExprNode* expr = nullptr;

  AssignmentNode(int _level);
  ~AssignmentNode();
  void printTo(ostream& os);
};

// <compound> -> { <statement> }
class CompoundNode : public StatementNode {
public:
  vector<StatementNode*> statements;

  CompoundNode(int _level);
  ~CompoundNode();
  void printTo(ostream& os);
};

// <if_statement> -> if ( <expr> ) <statement> [ else <statement> ]
class IfStatementNode : public StatementNode {
public:
  ExprNode* condition = nullptr;
  StatementNode* trueBranch = nullptr;
  StatementNode* falseBranch = nullptr;

  IfStatementNode(int _level);
  ~IfStatementNode();
  void printTo(ostream& os);
};

// <while_statement> -> while ( <expr> ) <statement>
class WhileStatementNode : public StatementNode {
public:
  ExprNode* condition = nullptr;
  StatementNode* body = nullptr;

  WhileStatementNode(int _level);
  ~WhileStatementNode();
  void printTo(ostream& os);
};

// <read_statement> -> read ID
class ReadStatementNode : public StatementNode {
public:
  string id;               // variable name

  ReadStatementNode(int _level);
  ~ReadStatementNode();
  void printTo(ostream& os);
};

// <write_statement> -> write <expr>
class WriteStatementNode : public StatementNode {
public:
  string content;
  int type = 0;

  WriteStatementNode(int _level);
  ~WriteStatementNode();
  void printTo(ostream& os);
};
// ---------------------------------------------------------------------
// <expr> -> <term> {{ (( + || - )) <term> }}
class ExprNode {
public:
  int level = 0;          // recursion level of this node
  TermNode* firstTerm = nullptr;
  int ExprOps = 0; // TOK_ADD_OP or TOK_SUB_OP
  TermNode* secondTerm = nullptr;

  ExprNode(int _level);
  ~ExprNode();
};
ostream& operator<<(ostream&, ExprNode&); // Node print operator
// ---------------------------------------------------------------------

class SimpleExprNode {
public:
    SimpleExprNode(int _level);
    ~SimpleExprNode();
    

    FactorNode* firstTerm;
    vector<int> restTermOps;
    vector<FactorNode*> restTerms;
};
ostream& operator<<(ostream&, SimpleExprNode&);

// <term> -> <factor> {{ (( * || / )) <factor> }}
class TermNode {
public:
  int level = 0;              // recursion level of this node
  FactorNode* firstFactor = nullptr;
  vector<int> restFactorOps;   // TOK_MULT_OP or TOK_DIV_OP
  vector<FactorNode*> restFactors;

  TermNode(int _level);
  ~TermNode();
};
ostream& operator<<(ostream&, TermNode&); // Node print operator
// ---------------------------------------------------------------------
// Abstract class. Base class for IdNode, IntLitNode, NestedExprNode.
// <factor> -> ID || INTLIT || ( <expr> )
class FactorNode {
public:
  int level = 0;                        // recursion level of this node

  virtual void printTo(ostream &os) = 0; // pure virtual method, makes the class Abstract
  virtual ~FactorNode();                 // labeling the destructor as virtual allows 
	                                       // the subclass destructors to be called
  
};
ostream& operator<<(ostream&, FactorNode&); // Node print operator
// ---------------------------------------------------------------------
// class IdNode (Identifier Node)
class IdNode : public FactorNode {
public:
    string id;

    IdNode(int _level, string name);
    ~IdNode();
    void printTo(ostream & os);
};
// ---------------------------------------------------------------------
// class IntLitNode (Integer Literal Node)
class IntLitNode : public FactorNode {
public:
    int int_literal = 0;

    IntLitNode(int _level, int value);
    ~IntLitNode();
    void printTo(ostream & os);
};

// class FloatLitNode (Floating Literal Node)
class FloatLitNode : public FactorNode {
public:
  float float_literal = 0.0;

  FloatLitNode(int _level, float value);
  ~FloatLitNode();
  void printTo(ostream &os);
};
// ---------------------------------------------------------------------
// class NestedExprNode (Nested Expression Node)
class NestedExprNode : public FactorNode {
public:
    ExprNode* exprPtr = nullptr;

    NestedExprNode(int _level, ExprNode* en);
    void printTo(ostream & os);
    ~NestedExprNode();
};

// class NotNode (Logical NOT Node)
class NotNode : public FactorNode {
public:
  FactorNode* factor = nullptr;

  NotNode(int _level, FactorNode* factor);
  ~NotNode();
  void printTo(ostream &os);
};

// class MinusNode (Unary Minus Node)
class MinusNode : public FactorNode {
public:
  FactorNode* factor = nullptr;

  MinusNode(int _level, FactorNode* factor);
  ~MinusNode();
  void printTo(ostream &os);
};

#endif /* PARSE_TREE_NODES_H */
