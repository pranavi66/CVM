#include "lexer.h"
#include "parser.h"
#include <iostream>
using namespace std;

void printAST(ASTNode* node, int indent = 0) {
    string pad(indent * 2, ' ');
    if (auto* n = dynamic_cast<ProgramNode*>(node)) {
        cout << pad << "Program" << endl;
        for (auto& s : n->statements) printAST(s.get(), indent + 1);
    } else if (auto* n = dynamic_cast<AssignNode*>(node)) {
        cout << pad << "Let: " << n->name << endl;
        printAST(n->value.get(), indent + 1);
    } else if (auto* n = dynamic_cast<PrintNode*>(node)) {
        cout << pad << "Print" << endl;
        printAST(n->value.get(), indent + 1);
    } else if (auto* n = dynamic_cast<BinOpNode*>(node)) {
        cout << pad << "BinOp: " << n->op << endl;
        printAST(n->left.get(),  indent + 1);
        printAST(n->right.get(), indent + 1);
    } else if (auto* n = dynamic_cast<NumberNode*>(node)) {
        cout << pad << "Number: " << n->value << endl;
    } else if (auto* n = dynamic_cast<VarNode*>(node)) {
        cout << pad << "Var: " << n->name << endl;
    } else if (auto* n = dynamic_cast<IfNode*>(node)) {
        cout << pad << "If" << endl;
        cout << pad << "  condition:" << endl;
        printAST(n->condition.get(), indent + 2);
        cout << pad << "  body:" << endl;
        for (auto& s : n->body) printAST(s.get(), indent + 2);
        if (!n->elseBody.empty()) {
            cout << pad << "  else:" << endl;
            for (auto& s : n->elseBody) printAST(s.get(), indent + 2);
        }
    } else if (auto* n = dynamic_cast<WhileNode*>(node)) {
        cout << pad << "While" << endl;
        cout << pad << "  condition:" << endl;
        printAST(n->condition.get(), indent + 2);
        cout << pad << "  body:" << endl;
        for (auto& s : n->body) printAST(s.get(), indent + 2);
    } else {
        cout << pad << "(unknown node)" << endl;
    }
}

int main() {
    string src =
        "let x = 10 + 5\n"
        "if x > 12 {\n"
        "  print x\n"
        "}";

    cout << "Source:\n" << src << "\n\nAST:\n";
    auto tokens = tokenize(src);
    auto ast    = parse(tokens);
    printAST(ast.get());
    return 0;
}