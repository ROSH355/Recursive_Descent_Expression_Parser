#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"
#include <memory>

class Parser {
public:
    explicit Parser(Lexer lex);
    // Parse the entire expression and return the root AST node.
    ASTPtr parse();

private:
    ASTPtr parseExpression();
    ASTPtr parseTerm();
    ASTPtr parseFactor();

    Lexer m_lexer;
    Token m_current;

    void consume(Token::Type type);
};

#endif // PARSER_H
