#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.h"
#include "parser.h"
#include "compiler.h"
#include "vm.h"
using namespace std;

string readFile(const string& path) {
    ifstream f(path);
    if (!f) throw runtime_error("Cannot open file: " + path);
    return string(istreambuf_iterator<char>(f), {});
}

void runSource(const string& src, bool debugBC = false) {
    auto tokens = tokenize(src);
    auto ast    = parse(tokens);
    auto bc     = compile(*ast);

    if (debugBC) {
        cout << "=== Bytecode ===" << endl;
        for (int i = 0; i < (int)bc.size(); i++) {
            cout << "[" << i << "] ";
            switch(bc[i].op) {
                case OpCode::PUSH:         cout << "PUSH " << bc[i].arg; break;
                case OpCode::LOAD:         cout << "LOAD " << bc[i].sarg; break;
                case OpCode::STORE:        cout << "STORE " << bc[i].sarg; break;
                case OpCode::ADD:          cout << "ADD"; break;
                case OpCode::SUB:          cout << "SUB"; break;
                case OpCode::MUL:          cout << "MUL"; break;
                case OpCode::DIV:          cout << "DIV"; break;
                case OpCode::NEG:          cout << "NEG"; break;
                case OpCode::CMP_EQ:       cout << "CMP_EQ"; break;
                case OpCode::CMP_LT:       cout << "CMP_LT"; break;
                case OpCode::CMP_GT:       cout << "CMP_GT"; break;
                case OpCode::PRINT:        cout << "PRINT"; break;
                case OpCode::INPUT:        cout << "INPUT " << bc[i].sarg; break;
                case OpCode::JMP:          cout << "JMP " << bc[i].arg; break;
                case OpCode::JMP_IF_FALSE: cout << "JMP_IF_FALSE " << bc[i].arg; break;
                case OpCode::HALT:         cout << "HALT"; break;
                default:                   cout << "???"; break;
            }
            cout << "\n";
        }
        cout << "=== Output ===" << endl;
    }

    VM vm;
    vm.run(bc);
}

int main(int argc, char* argv[]) {
    // file mode: ./cvm script.cvm [--debug]
    if (argc >= 2) {
        bool debug = (argc >= 3 && string(argv[2]) == "--debug");
        try {
            string src = readFile(argv[1]);
            runSource(src, debug);
        } catch (exception& e) {
            cerr << "Error: " << e.what() << endl;
            return 1;
        }
        return 0;
    }

    // REPL mode
    cout << "CVM++ REPL — type 'exit' to quit" << endl;
    string line, src;
    while (true) {
        cout << ">>> ";
        if (!getline(cin, line)) break;
        if (line == "exit") break;
        if (line.empty()) {
            if (!src.empty()) {
                try { runSource(src); }
                catch (exception& e) { cerr << "Error: " << e.what() << endl; }
                src.clear();
            }
            continue;
        }
        src += line + "\n";
    }
    return 0;
}