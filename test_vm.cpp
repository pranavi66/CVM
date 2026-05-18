#include "lexer.h"
#include "parser.h"
#include "compiler.h"
#include "vm.h"
#include <iostream>
using namespace std;

int main() {
    string src =
        "let i = 1\n"
        "while i < 6 {\n"
        "  print i\n"
        "  let i = i + 1\n"
        "}";

    cout << "Source:\n" << src << "\n\nOutput:\n";
    auto tokens = tokenize(src);
    auto ast    = parse(tokens);
    auto bc     = compile(*ast);
    VM vm;
    vm.run(bc);
    return 0;
}