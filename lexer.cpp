#include "lexer.h"  

#include <vector>
#include <string>
#include <stdexcept> // For runtime_error
#include <cctype> // For isdigit, isalpha, isalnum

using namespace std; 

// Reserved keywords
static const vector<string> KEYWORDS = { // List of keywords in the language
    "let",
    "print",
    "input",
    "if",
    "else",
    "while","true", "false"
};

// Check whether identifier is a keyword
static bool isKeyword(const string& s) { // Function to check if a string is a keyword
    for (auto& k : KEYWORDS) {
        if (k == s)
            return true;
    }

    return false;
}

// Main tokenizer
vector<Token> tokenize(const string& src) { // Function to tokenize the input source code
    vector<Token> tokens;

    size_t i = 0;

    while (i < src.size()) {

        char c = src[i];

        // Ignore whitespace
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') { 
            //we can also use isspace(c) instead of checking for each whitespace character
            i++;
            continue;
        }

        // Numbers
        if (isdigit(c)) {

            string num;

            while (i < src.size() && isdigit(src[i])) {
                num += src[i];
                i++;
            }

            tokens.push_back({TokenType::NUMBER, num});
            continue;
        }

        // Identifiers / Keywords
        // An identifier starts with a letter or underscore, followed by letters, digits, or underscores
        if (isalpha(c) || c == '_') {

            string id;

            while (i < src.size() &&
                  (isalnum(src[i]) || src[i] == '_')) {

                id += src[i];
                i++;
            }

            TokenType type;
            

            if (isKeyword(id))
                type = TokenType::KEYWORD;
            else
                type = TokenType::IDENT;

            tokens.push_back({type, id});
            continue;
        }

        // Two-character operators
        if (c == '=' &&
            i + 1 < src.size() &&
            src[i + 1] == '=') {

            tokens.push_back({TokenType::EQ, "=="});
            i += 2;
            continue;
        }

        // Single-character tokens
        switch (c) {

            case '=':
                tokens.push_back({TokenType::ASSIGN, "="});
                break;

            case '<':
                tokens.push_back({TokenType::LT, "<"});
                break;
            case '>':
                 tokens.push_back({TokenType::GT, ">"}); 
                 break;

            case '+':
                tokens.push_back({TokenType::PLUS, "+"});
                break;

            case '-':
                tokens.push_back({TokenType::MINUS, "-"});
                break;

            case '*':
                tokens.push_back({TokenType::STAR, "*"});
                break;

            case '/':
                tokens.push_back({TokenType::SLASH, "/"});
                break;

            case '{':
                tokens.push_back({TokenType::LBRACE, "{"});
                break;

            case '}':
                tokens.push_back({TokenType::RBRACE, "}"});
                break;

            case ';':
                tokens.push_back({TokenType::SEMI, ";"});
                break;

            default:
                throw runtime_error(
                    string("Unknown character: ") + c
                );
        }

        i++;
    }

    // End token
    tokens.push_back({TokenType::END, ""});

    return tokens;
}