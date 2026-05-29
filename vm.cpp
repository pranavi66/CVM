#include "vm.h"
#include <iostream>
#include <stdexcept>
using namespace std;

void VM::run(const vector<Instruction>& code) {
    vector<int>    stack;      // operand stack
    map<string,int> vars;      // variable store
    int ip = 0;                // instruction pointer
 // trading DSL defaults — safe for regular CVM++ scripts too
    vars["cash"]      = 10000;
    vars["portfolio"] = 0;
    while (ip < (int)code.size()) {
        const Instruction& ins = code[ip];

        switch (ins.op) {

            case OpCode::PUSH:
                stack.push_back(ins.arg);
                ip++;
                break;

            case OpCode::LOAD: {
                auto it = vars.find(ins.sarg);
                if (it == vars.end())
                    throw runtime_error("Undefined variable: " + ins.sarg);
                stack.push_back(it->second);
                ip++;
                break;
            }

            case OpCode::STORE:
                vars[ins.sarg] = stack.back();
                stack.pop_back();
                ip++;
                break;

            case OpCode::ADD: {
                int b = stack.back(); stack.pop_back();
                int a = stack.back(); stack.pop_back();
                stack.push_back(a + b);
                ip++;
                break;
            }
            case OpCode::SUB: {
                int b = stack.back(); stack.pop_back();
                int a = stack.back(); stack.pop_back();
                stack.push_back(a - b);
                ip++;
                break;
            }
            case OpCode::MUL: {
                int b = stack.back(); stack.pop_back();
                int a = stack.back(); stack.pop_back();
                stack.push_back(a * b);
                ip++;
                break;
            }
            case OpCode::DIV: {
                int b = stack.back(); stack.pop_back();
                int a = stack.back(); stack.pop_back();
                if (b == 0) throw runtime_error("Division by zero");
                stack.push_back(a / b);
                ip++;
                break;
            }

            case OpCode::NEG:
                stack.back() = -stack.back();
                ip++;
                break;

            case OpCode::CMP_EQ: {
                int b = stack.back(); stack.pop_back();
                int a = stack.back(); stack.pop_back();
                stack.push_back(a == b ? 1 : 0);
                ip++;
                break;
            }
            case OpCode::CMP_LT: {
                int b = stack.back(); stack.pop_back();
                int a = stack.back(); stack.pop_back();
                stack.push_back(a < b ? 1 : 0);
                ip++;
                break;
            }
            case OpCode::CMP_GT: {
                int b = stack.back(); stack.pop_back();
                int a = stack.back(); stack.pop_back();
                stack.push_back(a > b ? 1 : 0);
                ip++;
                break;
            }

            case OpCode::PRINT:
                cout << stack.back() << "\n";
                stack.pop_back();
                ip++;
                break;

            case OpCode::INPUT: {
                int val;
                cout << "input> ";
                cin >> val;
                vars[ins.sarg] = val;
                ip++;
                break;
            }

            case OpCode::JMP:
                ip = ins.arg;
                break;

            case OpCode::JMP_IF_FALSE: {
                int v = stack.back(); stack.pop_back();
                ip = (v == 0) ? ins.arg : ip + 1;
                break;
            }

            case OpCode::HALT:
                return;

            default:
                throw runtime_error("Unknown opcode");
        }
    }
}