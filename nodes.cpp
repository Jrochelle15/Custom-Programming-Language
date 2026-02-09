

#include "nodes.h"

bool printDelete = false;   // shall we print deleting the tree?

// ---------------------------------------------------------------------
// Indent according to tree level
static void indent(int level) {
  for (int i = 0; i < level; i++)
    cout << ("| ");
}
// ---------------------------------------------------------------------
StatementNode::StatementNode(int level, FactorNode* expr) {
    _level = level;
    this->expr = expr;
}
StatementNode::~StatementNode() {
    if(printDelete)
        cout << "Deleting FactorNode:StatementNode " << endl;
    delete expr;
    expr = nullptr;
}
void StatementNode::printTo(ostream& os) {
    os << "(STATEMENT ";
    expr->printTo(os);
    os << ") ";
}

AssignmentNode::AssignmentNode(int level, IdNode* id, FactorNode* expr) : StatementNode(level, expr) {
    this->id = id;
    this->expr = expr;
}
AssignmentNode::~AssignmentNode() {
    if(printDelete)
        cout << "Deleting AssignmentNode " << endl;
    delete id;
    id = nullptr;
    delete expr;
    expr = nullptr;
}
void AssignmentNode::printTo(ostream& os) {
    os << "(ASSIGNMENT ";
    id->printTo(os);
    os << " = ";
    expr->printTo(os);
    os << ") ";
}

CompoundNode::CompoundNode(int level) : StatementNode(level, nullptr) {
    // No expression associated with a compound statement
}
CompoundNode::~CompoundNode() {
    if(printDelete)
        cout << "Deleting CompoundStatementNode " << endl;
    for (auto stmt : statements) {
        delete stmt;
    }
    statements.clear();
}
void CompoundNode::addStatement(StatementNode* stmt) {
    statements.push_back(stmt);
}
void CompoundStatementNode::printTo(ostream& os) {
    os << "(COMPOUND_STATEMENT ";
    for (auto stmt : statements) {
        stmt->printTo(os);
    }
    os << ") ";
}

IfNode::IfNode(int level, FactorNode* condition, StatementNode* thenStmt, StatementNode* elseStmt)
    : StatementNode(level, nullptr), condition(condition), thenStmt(thenStmt), elseStmt(elseStmt) {}
IfNode::~IfNode() {
    if(printDelete)
        cout << "Deleting IfNode " << endl;
    delete condition;
    condition = nullptr;
    delete thenStmt;
    thenStmt = nullptr;
    if (elseStmt) {
        delete elseStmt;
        elseStmt = nullptr;
    }
}
void IfNode::printTo(ostream& os) {
    os << "(IF ";
    condition->printTo(os);
    os << " THEN ";
    thenStmt->printTo(os);
    if (elseStmt) {
        os << " ELSE ";
        elseStmt->printTo(os);
    }
    os << ") ";
}

WhileNode::WhileNode(int level, FactorNode* condition, StatementNode* body)
    : StatementNode(level, nullptr), condition(condition), body(body) {}
WhileNode::~WhileNode() {
    if(printDelete)
        cout << "Deleting WhileNode " << endl;
    delete condition;
    condition = nullptr;
    delete body;
    body = nullptr;
}
void WhileNode::printTo(ostream& os) {
    os << "(WHILE ";
    condition->printTo(os);
    os << " DO ";
    body->printTo(os);
    os << ") ";
}

ReadNode::ReadNode(int level, IdNode* id) : StatementNode(level, nullptr) {
    this->id = id;
}
ReadNode::~ReadNode() {
    if(printDelete)
        cout << "Deleting ReadNode " << endl;
    delete id;
    id = nullptr;
}
void ReadNode::printTo(ostream& os) {
    os << "(READ ";
    id->printTo(os);
    os << ") ";
}

WriteNode::WriteNode(int level, FactorNode* expr) : StatementNode(level, expr) {
    this->expr = expr;
}
WriteNode::~WriteNode() {
    if(printDelete)
        cout << "Deleting WriteNode " << endl;
    delete expr;
    expr = nullptr;
}
void WriteNode::printTo(ostream& os) {
    os << "(WRITE ";
    expr->printTo(os);
    os << ") ";
}
//---------------------------------------------------------------------
FactorNode::~FactorNode() {}
// Uses double dispatch to call the overloaded method printTo in the 
// FactorNodes: IdNode, IntLitNode, and NestedExprNode
ostream& operator<<(ostream& os, FactorNode& fn) {
  os << endl; indent(fn._level); os << "(factor ";
  fn.printTo(os);
  os << endl; indent(fn._level); os << "factor) ";
	return os;
}
// ---------------------------------------------------------------------
IdNode::IdNode(int level, string name) {
  _level = level;
  id = new string(name);
}
IdNode::~IdNode() {
  if(printDelete) 
    cout << "Deleting FactorNode:IdNode " << endl;
	delete id;
	id = nullptr;
}
void IdNode::printTo(ostream& os) {
	os << "(IDENT: " << *id << ") ";
}
// ---------------------------------------------------------------------
IntLitNode::IntLitNode(int level, int value) {
  _level = level;
  int_literal = value;
}
IntLitNode::~IntLitNode() {
  if(printDelete)
    cout << "Deleting FactorNode:IntLitNode " << endl;
	  // Nothing to do since the only members are not pointers
}
void IntLitNode::printTo(ostream& os) {
	os << "(INTLIT: " << int_literal << ") ";
}

FloatLitNode::FloatLitNode(int level, double value) {
    _level = level;
    float_literal = value;
}
FloatLitNode::~FloatLitNode() {
    if(printDelete)
        cout << "Deleting FactorNode:FloatLitNode " << endl;
    // Nothing to do since the only members are not pointers
}
void FloatLitNode::printTo(ostream& os) {
    os << "(FLOATLIT: " << float_literal << ") ";
}
// ---------------------------------------------------------------------
NestedExprNode::NestedExprNode(int level, ExprNode* en) {
  _level = level;
	exprPtr = en;
}
void NestedExprNode::printTo(ostream& os) {
	os << *exprPtr;
}
NestedExprNode::~NestedExprNode() {
  if(printDelete)
    cout << "Deleting FactorNode:NestedExprNode " << endl;
  delete exprPtr;
  exprPtr = nullptr;
}

NotNode::NotNode(int level, FactorNode* operand) {
    _level = level;
    this->operand = operand;
}
NotNode::~NotNode() {
    if(printDelete)
        cout << "Deleting FactorNode:NotNode " << endl;
    delete operand;
    operand = nullptr;
}
void NotNode::printTo(ostream& os) {
    os << "(NOT ";
    operand->printTo(os);
    os << ") ";
}

MinusNode::MinusNode(int level, FactorNode* operand) {
    _level = level;
    this->operand = operand;
}
MinusNode::~MinusNode() {
    if(printDelete)
        cout << "Deleting FactorNode:MinusNode " << endl;
    delete operand;
    operand = nullptr;
}
void MinusNode::printTo(ostream& os) {
    os << "(MINUS ";
    operand->printTo(os);
    os << ") ";
}
// ---------------------------------------------------------------------
TermNode::TermNode(int level) {
  _level = level;
}
ostream& operator<<(ostream& os, TermNode& tn) {
  os << endl; indent(tn._level); os << "(term ";
	os << *(tn.firstFactor);

	int length = tn.restFactorOps.size();
	for (int i = 0; i < length; ++i) {
		int op = tn.restFactorOps[i];
    if (op == TOK_MULT_OP) {
      os << endl; indent(tn._level); os << "* ";
    } else {
      os << endl; indent(tn._level); os << "/ ";
    }
		os << *(tn.restFactors[i]);
	}
  os << endl; indent(tn._level); os << "term) ";
	return os;
}
TermNode::~TermNode() {
  if(printDelete)
    cout << "Deleting TermNode " << endl;
	delete firstFactor;
	firstFactor = nullptr;

	int length = restFactorOps.size();
	for (int i = 0; i < length; ++i) {
		delete restFactors[i];
		restFactors[i] = nullptr;
	}
}
// ---------------------------------------------------------------------
ExprNode::ExprNode(int level) {
  _level = level;
}
ostream& operator<<(ostream& os, ExprNode& en) {
  os << endl; indent(en._level); os << "(expr ";
	os << *(en.firstTerm);

	int length = en.restTermOps.size();
	for (int i = 0; i < length; ++i) {
		int op = en.restTermOps[i];
    if (op == TOK_ADD_OP) {
      os << endl; indent(en._level); os << "+ ";
    } else {
      os << endl; indent(en._level); os << "- ";
    }
		os << *(en.restTerms[i]);
	}
  os << endl; indent(en._level); os << "expr) ";
	return os;
}
ExprNode::~ExprNode() {
  if(printDelete)
    cout << "Deleting ExprNode " << endl;
	delete firstTerm;
	firstTerm = nullptr;

	int length = restTermOps.size();
	for (int i = 0; i < length; ++i) {
		delete restTerms[i];
		restTerms[i] = nullptr;
	}
}

SimpleExprNode::SimpleExprNode(int level, FactorNode* firstTerm) {
    _level = level;
    this->firstTerm = firstTerm;
}

SimpleExprNode::~SimpleExprNode() {
    if(printDelete)
        cout << "Deleting SimpleExprNode " << endl;
    delete firstTerm;
    firstTerm = nullptr;

    int length = restTerms.size();
    for (int i = 0; i < length; ++i) {
        delete restTerms[i];
        restTerms[i] = nullptr;
    }
}

void SimpleExprNode::addTerm(int op, FactorNode* term) {
    restTermOps.push_back(op);
    restTerms.push_back(term);
}

void SimpleExprNode::printTo(ostream& os) {
    os << endl; indent(_level); os << "(expr ";
    firstTerm->printTo(os);

    int length = restTermOps.size();
    for (int i = 0; i < length; ++i) {
        int op = restTermOps[i];
        if (op == TOK_ADD_OP) {
            os << endl; indent(_level); os << "+ ";
        } else {
            os << endl; indent(_level); os << "- ";
        }
        restTerms[i]->printTo(os);
    }
    os << endl; indent(_level); os << "expr) ";
}