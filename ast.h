#pragma once
#include <string>
#include <vector>
#include <memory>
using namespace std;

struct ASTNode {
    virtual ~ASTNode() = default;
};
using NodePtr = unique_ptr<ASTNode>;

struct NumberNode : ASTNode {
    int value;
    NumberNode(int v) : value(v) {}
};

struct BoolNode : ASTNode {
    bool value;
    BoolNode(bool v) : value(v) {}
};

struct VarNode : ASTNode {
    string name;
    VarNode(const string& n) : name(n) {}
};

struct BinOpNode : ASTNode {
    string op;
    NodePtr left, right;
    BinOpNode(const string& o, NodePtr l, NodePtr r)
        : op(o), left(move(l)), right(move(r)) {}
};

struct UnaryMinusNode : ASTNode {
    NodePtr operand;
    UnaryMinusNode(NodePtr o) : operand(move(o)) {}
};

struct AssignNode : ASTNode {
    string name;
    NodePtr value;
    AssignNode(const string& n, NodePtr v)
        : name(n), value(move(v)) {}
};

struct PrintNode : ASTNode {
    NodePtr value;
    PrintNode(NodePtr v) : value(move(v)) {}
};

struct InputNode : ASTNode {
    string name;
    InputNode(const string& n) : name(n) {}
};

struct IfNode : ASTNode {
    NodePtr condition;
    vector<NodePtr> body;
    vector<NodePtr> elseBody;
};

struct WhileNode : ASTNode {
    NodePtr condition;
    vector<NodePtr> body;
};

struct ProgramNode : ASTNode {
    vector<NodePtr> statements;
};