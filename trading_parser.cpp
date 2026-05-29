#include "trading_parser.h"
#include <stdexcept>
using namespace std;

struct TradingParser {
    const vector<Token>& tokens;
    size_t pos = 0;
    NodePtr pending = nullptr;    // ← holds the second statement for buy

    Token peek() { return tokens[pos]; }
    Token eat()  { return tokens[pos++]; }

    Token expect(TokenType t) {
        if (tokens[pos].type != t)
            throw runtime_error(
                string("Trading parse error: unexpected '") + tokens[pos].value + "'"
            );
        return tokens[pos++];
    }

    bool matchKw(const string& kw) {
        return peek().type == TokenType::KEYWORD && peek().value == kw;
    }

    unique_ptr<ProgramNode> parseProgram() {
        auto prog = make_unique<ProgramNode>();
        while (peek().type != TokenType::END) {
            prog->statements.push_back(parseStmt());
            // if buy generated a second statement flush it here
            if (pending) {
                prog->statements.push_back(move(pending));
                pending = nullptr;
            }
        }
        return prog;
    }

    NodePtr parseStmt() {

        // price AAPL = 150  →  let AAPL = 150
        if (matchKw("price")) {
            eat();
            string name = expect(TokenType::IDENT).value;
            expect(TokenType::ASSIGN);
            return make_unique<AssignNode>(name, parseExpr());
        }

        // buy AAPL 10
        // →  portfolio = portfolio + (AAPL * 10)
        //    cash      = cash      - (AAPL * 10)
        if (matchKw("buy")) {
            eat();
            string stock = expect(TokenType::IDENT).value;
            int qty = stoi(expect(TokenType::NUMBER).value);

            // portfolio = portfolio + (stock * qty)
            auto portUpdate = make_unique<AssignNode>("portfolio",
                make_unique<BinOpNode>("+",
                    make_unique<VarNode>("portfolio"),
                    make_unique<BinOpNode>("*",
                        make_unique<VarNode>(stock),
                        make_unique<NumberNode>(qty))));

            // cash = cash - (stock * qty)  ← queue as pending
            pending = make_unique<AssignNode>("cash",
                make_unique<BinOpNode>("-",
                    make_unique<VarNode>("cash"),
                    make_unique<BinOpNode>("*",
                        make_unique<VarNode>(stock),
                        make_unique<NumberNode>(qty))));

            return portUpdate;
        }

        // sell MSFT 5
        // →  cash = cash + (MSFT * 5)
        if (matchKw("sell")) {
            eat();
            string stock = expect(TokenType::IDENT).value;
            int qty = stoi(expect(TokenType::NUMBER).value);
            return make_unique<AssignNode>("cash",
                make_unique<BinOpNode>("+",
                    make_unique<VarNode>("cash"),
                    make_unique<BinOpNode>("*",
                        make_unique<VarNode>(stock),
                        make_unique<NumberNode>(qty))));
        }

        // show x  →  print x
        if (matchKw("show")) {
            eat();
            string name = expect(TokenType::IDENT).value;
            return make_unique<PrintNode>(make_unique<VarNode>(name));
        }

        if (matchKw("let"))   return parseLet();
        if (matchKw("print")) return parsePrint();
        if (matchKw("if"))    return parseIf();
        if (matchKw("while")) return parseWhile();

        throw runtime_error(
            string("Unknown statement: '") + peek().value + "'"
        );
    }

    NodePtr parseLet() {
        eat();
        string name = expect(TokenType::IDENT).value;
        expect(TokenType::ASSIGN);
        return make_unique<AssignNode>(name, parseExpr());
    }

    NodePtr parsePrint() {
        eat();
        return make_unique<PrintNode>(parseExpr());
    }

    NodePtr parseIf() {
        eat();
        auto node = make_unique<IfNode>();
        node->condition = parseExpr();
        expect(TokenType::LBRACE);
        while (peek().type != TokenType::RBRACE && peek().type != TokenType::END) {
            node->body.push_back(parseStmt());
            if (pending) {
                node->body.push_back(move(pending));
                pending = nullptr;
            }
        }
        expect(TokenType::RBRACE);
        if (matchKw("else")) {
            eat();
            expect(TokenType::LBRACE);
            while (peek().type != TokenType::RBRACE && peek().type != TokenType::END) {
                node->elseBody.push_back(parseStmt());
                if (pending) {
                    node->elseBody.push_back(move(pending));
                    pending = nullptr;
                }
            }
            expect(TokenType::RBRACE);
        }
        return node;
    }

    NodePtr parseWhile() {
        eat();
        auto node = make_unique<WhileNode>();
        node->condition = parseExpr();
        expect(TokenType::LBRACE);
        while (peek().type != TokenType::RBRACE && peek().type != TokenType::END) {
            node->body.push_back(parseStmt());
            if (pending) {
                node->body.push_back(move(pending));
                pending = nullptr;
            }
        }
        expect(TokenType::RBRACE);
        return node;
    }

    NodePtr parseExpr()  { return parseCmp(); }
    NodePtr parseCmp() {
        NodePtr left = parseAdd();
        while (peek().type==TokenType::EQ||peek().type==TokenType::LT||peek().type==TokenType::GT) {
            string op = eat().value;
            left = make_unique<BinOpNode>(op, move(left), parseAdd());
        }
        return left;
    }
    NodePtr parseAdd() {
        NodePtr left = parseMul();
        while (peek().type==TokenType::PLUS||peek().type==TokenType::MINUS) {
            string op = eat().value;
            left = make_unique<BinOpNode>(op, move(left), parseMul());
        }
        return left;
    }
    NodePtr parseMul() {
        NodePtr left = parsePrimary();
        while (peek().type==TokenType::STAR||peek().type==TokenType::SLASH) {
            string op = eat().value;
            left = make_unique<BinOpNode>(op, move(left), parsePrimary());
        }
        return left;
    }
    NodePtr parsePrimary() {
        Token tk = peek();
        if (tk.type==TokenType::NUMBER) { eat(); return make_unique<NumberNode>(stoi(tk.value)); }
        if (tk.type==TokenType::KEYWORD&&tk.value=="true")  { eat(); return make_unique<BoolNode>(true); }
        if (tk.type==TokenType::KEYWORD&&tk.value=="false") { eat(); return make_unique<BoolNode>(false); }
        if (tk.type==TokenType::IDENT)  { eat(); return make_unique<VarNode>(tk.value); }
        if (tk.type==TokenType::MINUS)  { eat(); return make_unique<UnaryMinusNode>(parsePrimary()); }
        throw runtime_error(string("Unexpected token: '") + tk.value + "'");
    }
};

unique_ptr<ProgramNode> parseTradingScript(const vector<Token>& tokens) {
    TradingParser p{tokens};
    return p.parseProgram();
}