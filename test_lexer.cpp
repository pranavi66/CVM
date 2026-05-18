#include "lexer.h"
#include <iostream>
using namespace std;

int main() {
    string src = "let x = 10 + 5";
    cout << "Input: " << src << endl;
    cout << "Tokens:" << endl;

    auto tokens = tokenize(src);
    for (auto& t : tokens) {
        string typeName;
        switch (t.type) {
            case TokenType::NUMBER:  typeName = "NUMBER";  break;
            case TokenType::IDENT:   typeName = "IDENT";   break;
            case TokenType::KEYWORD: typeName = "KEYWORD"; break;
            case TokenType::PLUS:    typeName = "PLUS";    break;
            case TokenType::MINUS:   typeName = "MINUS";   break;
            case TokenType::STAR:    typeName = "STAR";    break;
            case TokenType::SLASH:   typeName = "SLASH";   break;
            case TokenType::ASSIGN:  typeName = "ASSIGN";  break;
            case TokenType::EQ:      typeName = "EQ";      break;
            case TokenType::LT:      typeName = "LT";      break;
            case TokenType::GT:      typeName = "GT";      break;
            case TokenType::LBRACE:  typeName = "LBRACE";  break;
            case TokenType::RBRACE:  typeName = "RBRACE";  break;
            case TokenType::SEMI:    typeName = "SEMI";    break;
            case TokenType::END:     typeName = "END";     break;
        }
        cout << "  " << typeName << " -> \"" << t.value << "\"" << endl;
    }
    return 0;
}