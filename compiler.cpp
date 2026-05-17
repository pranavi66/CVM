#include "compiler.h"
#include <stdexcept>
using namespace std;

struct Compiler {
    vector<Instruction> code;

    // ── helpers ──────────────────────────────────────────────
    int emit(OpCode op, int arg = 0, const string& sarg = "") {
        code.push_back({op, arg, sarg});
        return (int)code.size() - 1;   // returns index of emitted instruction
    }

    int here() { return (int)code.size(); }

    void patch(int idx, int target) {
        code[idx].arg = target;         // backpatch a jump target
    }

    // ── expressions ──────────────────────────────────────────
    void compileExpr(ASTNode* node) {
        if (auto* n = dynamic_cast<NumberNode*>(node)) {
            emit(OpCode::PUSH, n->value);
            return;
        }
        if (auto* n = dynamic_cast<BoolNode*>(node)) {
            emit(OpCode::PUSH, n->value ? 1 : 0);
            return;
        }
        if (auto* n = dynamic_cast<VarNode*>(node)) {
            emit(OpCode::LOAD, 0, n->name);
            return;
        }
        if (auto* n = dynamic_cast<UnaryMinusNode*>(node)) {
            compileExpr(n->operand.get());
            emit(OpCode::NEG);
            return;
        }
        if (auto* n = dynamic_cast<BinOpNode*>(node)) {
            compileExpr(n->left.get());
            compileExpr(n->right.get());
            if      (n->op == "+")  emit(OpCode::ADD);
            else if (n->op == "-")  emit(OpCode::SUB);
            else if (n->op == "*")  emit(OpCode::MUL);
            else if (n->op == "/")  emit(OpCode::DIV);
            else if (n->op == "==") emit(OpCode::CMP_EQ);
            else if (n->op == "<")  emit(OpCode::CMP_LT);
            else if (n->op == ">")  emit(OpCode::CMP_GT);
            else throw runtime_error("Unknown operator: " + n->op);
            return;
        }
        throw runtime_error("Unknown AST node in expression");
    }

    // ── statements ───────────────────────────────────────────
    void compileStmt(ASTNode* node) {
        if (auto* n = dynamic_cast<AssignNode*>(node)) {
            compileExpr(n->value.get());
            emit(OpCode::STORE, 0, n->name);
            return;
        }
        if (auto* n = dynamic_cast<PrintNode*>(node)) {
            compileExpr(n->value.get());
            emit(OpCode::PRINT);
            return;
        }
        if (auto* n = dynamic_cast<InputNode*>(node)) {
            emit(OpCode::INPUT, 0, n->name);
            return;
        }
        if (auto* n = dynamic_cast<IfNode*>(node)) {
            // compile condition
            compileExpr(n->condition.get());
            int jmpFalse = emit(OpCode::JMP_IF_FALSE); // placeholder

            // compile if-body
            for (auto& s : n->body) compileStmt(s.get());

            if (!n->elseBody.empty()) {
                int jmpEnd = emit(OpCode::JMP);         // skip else
                patch(jmpFalse, here());                // false → else start
                for (auto& s : n->elseBody) compileStmt(s.get());
                patch(jmpEnd, here());                  // end of if → after else
            } else {
                patch(jmpFalse, here());                // false → after body
            }
            return;
        }
        if (auto* n = dynamic_cast<WhileNode*>(node)) {
            int loopStart = here();                     // top of loop
            compileExpr(n->condition.get());
            int jmpOut = emit(OpCode::JMP_IF_FALSE);    // placeholder

            for (auto& s : n->body) compileStmt(s.get());
            emit(OpCode::JMP, loopStart);               // back to top
            patch(jmpOut, here());                      // false → after loop
            return;
        }
        throw runtime_error("Unknown AST node in statement");
    }

    // ── entry point ──────────────────────────────────────────
    vector<Instruction> run(const ProgramNode& prog) {
        for (auto& s : prog.statements) compileStmt(s.get());
        emit(OpCode::HALT);
        return code;
    }
};

vector<Instruction> compile(const ProgramNode& program) {
    Compiler c;
    return c.run(program);
}