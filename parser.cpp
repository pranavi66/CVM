#include "parser.h"
#include <stdexcept>
using namespace std;

struct Parser {
    const vector<Token>& tokens;
    size_t pos = 0;

    Token peek() {
        return tokens[pos];
    }

    Token eat() {
        return tokens[pos++];
    }

    Token expect(TokenType t) {
        if (tokens[pos].type != t)
            throw runtime_error(
                string("Parse error: unexpected token '") + tokens[pos].value + "'"
            );
        return tokens[pos++];
    }

    bool matchKw(const string& kw) {
        return peek().type == TokenType::KEYWORD
            && peek().value == kw;
    }

    // ── Entry point ──────────────────────────────────────────
    unique_ptr<ProgramNode> parseProgram() {
        auto prog = make_unique<ProgramNode>();
        while (peek().type != TokenType::END)
            prog->statements.push_back(parseStmt());
        return prog;
    }

    // ── Statements ───────────────────────────────────────────
    NodePtr parseStmt() {
        if (matchKw("let"))   return parseLet();
        if (matchKw("print")) return parsePrint();
        if (matchKw("input")) return parseInput();
        if (matchKw("if"))    return parseIf();
        if (matchKw("while")) return parseWhile();
        throw runtime_error(
            string("Unknown statement: '") + peek().value + "'"
        );
    }

    NodePtr parseLet() {
        eat();  // 'let'
        string name = expect(TokenType::IDENT).value;
        expect(TokenType::ASSIGN);
        NodePtr val = parseExpr();
        return make_unique<AssignNode>(name, move(val));
    }

    NodePtr parsePrint() {
        eat();  // 'print'
        NodePtr val = parseExpr();
        return make_unique<PrintNode>(move(val));
    }

    NodePtr parseInput() {
        eat();  // 'input'
        string name = expect(TokenType::IDENT).value;
        return make_unique<InputNode>(name);
    }

    NodePtr parseIf() {
        eat();  // 'if'
        auto node = make_unique<IfNode>();
        node->condition = parseExpr();
        expect(TokenType::LBRACE);
        while (peek().type != TokenType::RBRACE
            && peek().type != TokenType::END)
            node->body.push_back(parseStmt());
        expect(TokenType::RBRACE);

        if (matchKw("else")) {
            eat();  // 'else'
            expect(TokenType::LBRACE);
            while (peek().type != TokenType::RBRACE
                && peek().type != TokenType::END)
                node->elseBody.push_back(parseStmt());
            expect(TokenType::RBRACE);
        }
        return node;
    }

    NodePtr parseWhile() {
        eat();  // 'while'
        auto node = make_unique<WhileNode>();
        node->condition = parseExpr();
        expect(TokenType::LBRACE);
        while (peek().type != TokenType::RBRACE
            && peek().type != TokenType::END)
            node->body.push_back(parseStmt());
        expect(TokenType::RBRACE);
        return node;
    }

    // ── Expressions (lowest → highest precedence) ────────────
    NodePtr parseExpr() {
        return parseCmp();
    }

    NodePtr parseCmp() {
        NodePtr left = parseAdd();
        while (peek().type == TokenType::EQ
            || peek().type == TokenType::LT
            || peek().type == TokenType::GT) {
            string op = eat().value;
            NodePtr right = parseAdd();
            left = make_unique<BinOpNode>(op, move(left), move(right));
        }
        return left;
    }

    NodePtr parseAdd() {
        NodePtr left = parseMul();
        while (peek().type == TokenType::PLUS
            || peek().type == TokenType::MINUS) {
            string op = eat().value;
            NodePtr right = parseMul();
            left = make_unique<BinOpNode>(op, move(left), move(right));
        }
        return left;
    }

    NodePtr parseMul() {
        NodePtr left = parsePrimary();
        while (peek().type == TokenType::STAR
            || peek().type == TokenType::SLASH) {
            string op = eat().value;
            NodePtr right = parsePrimary();
            left = make_unique<BinOpNode>(op, move(left), move(right));
        }
        return left;
    }

    NodePtr parsePrimary() {
        Token tk = peek();

        if (tk.type == TokenType::NUMBER) {
            eat();
            return make_unique<NumberNode>(stoi(tk.value));
        }
        if (tk.type == TokenType::KEYWORD && tk.value == "true") {
            eat();
            return make_unique<BoolNode>(true);
        }
        if (tk.type == TokenType::KEYWORD && tk.value == "false") {
            eat();
            return make_unique<BoolNode>(false);
        }
        if (tk.type == TokenType::IDENT) {
            eat();
            return make_unique<VarNode>(tk.value);
        }
        if (tk.type == TokenType::MINUS) {
            eat();
            return make_unique<UnaryMinusNode>(parsePrimary());
        }
        throw runtime_error(
            string("Unexpected token in expression: '") + tk.value + "'"
        );
    }
};

unique_ptr<ProgramNode> parse(const vector<Token>& tokens) {
    Parser p{tokens};
    return p.parseProgram();
}