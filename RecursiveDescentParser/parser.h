#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"
#include <memory>

class Parser {
public:
    explicit Parser(Lexer lex, SymbolTable symbols = SymbolTable());
    // Parse a single statement (assignment or expression).
    // Returns nullptr for empty statements; after END token, returns nullptr.
    ASTPtr parseStatement();
    // Parse an expression (used within statements).
    ASTPtr parseExpression();
    // Get reference to symbol table (for persistence across statements)
    SymbolTable &getSymbols();

private:
    ASTPtr parseTerm();
    ASTPtr parseFactor();

    Lexer m_lexer;
    Token m_current;
    SymbolTable m_symbols;

    void consume(Token::Type type);
    void advanceToken();
};

#endif // PARSER_H
