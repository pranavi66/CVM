#include "lexer.h"
#include "parser.h"
#include "compiler.h"
#include <iostream>
using namespace std;

int main() {
    string src =
        "let x = 10 + 5\n"
        "if x > 12 {\n"
        "  print x\n"
        "}";

    cout << "Source:\n" << src << "\n\nBytecode:\n";
    auto tokens = tokenize(src);
    auto ast    = parse(tokens);
    auto bc     = compile(*ast);

    for (int i = 0; i < (int)bc.size(); i++) {
        cout << "[" << i << "] ";
        switch (bc[i].op) {
            case OpCode::PUSH:         cout << "PUSH " << bc[i].arg;          break;
            case OpCode::LOAD:         cout << "LOAD " << bc[i].sarg;         break;
            case OpCode::STORE:        cout << "STORE " << bc[i].sarg;        break;
            case OpCode::ADD:          cout << "ADD";                          break;
            case OpCode::SUB:          cout << "SUB";                          break;
            case OpCode::MUL:          cout << "MUL";                          break;
            case OpCode::DIV:          cout << "DIV";                          break;
            case OpCode::NEG:          cout << "NEG";                          break;
            case OpCode::CMP_EQ:       cout << "CMP_EQ";                       break;
            case OpCode::CMP_LT:       cout << "CMP_LT";                       break;
            case OpCode::CMP_GT:       cout << "CMP_GT";                       break;
            case OpCode::PRINT:        cout << "PRINT";                        break;
            case OpCode::INPUT:        cout << "INPUT " << bc[i].sarg;        break;
            case OpCode::JMP:          cout << "JMP " << bc[i].arg;           break;
            case OpCode::JMP_IF_FALSE: cout << "JMP_IF_FALSE " << bc[i].arg;  break;
            case OpCode::HALT:         cout << "HALT";                         break;
        }
        cout << endl;
    }
    return 0;
}