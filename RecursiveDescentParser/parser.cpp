#include "parser.h"
#include <stdexcept>

Parser::Parser(Lexer lex) : m_lexer(std::move(lex)) {
    // initialize current token by peeking at lexer
    m_current = m_lexer.peek();
}

ASTPtr Parser::parse() {
    ASTPtr node = parseExpression();
    if (m_current.type != Token::END) {
        throw std::runtime_error("Unexpected token after end of expression");
    }
    return node;
}

void Parser::consume(Token::Type type) {
    if (m_current.type == type) {
        // advance lexer and update current token
        m_lexer.get();
        m_current = m_lexer.peek();
    } else {
        throw std::runtime_error("Unexpected token in input");
    }
}

ASTPtr Parser::parseExpression() {
    // Expression -> Term ((PLUS | MINUS) Term)*
    ASTPtr node = parseTerm();
    while (m_current.type == Token::PLUS || m_current.type == Token::MINUS) {
        char op = (m_current.type == Token::PLUS) ? '+' : '-';
        consume(m_current.type);
        ASTPtr right = parseTerm();
        node = std::make_unique<BinaryOpNode>(op, std::move(node), std::move(right));
    }
    return node;
}

ASTPtr Parser::parseTerm() {
    // Term -> Factor ((MUL | DIV) Factor)*
    ASTPtr node = parseFactor();
    while (m_current.type == Token::MUL || m_current.type == Token::DIV) {
        char op = (m_current.type == Token::MUL) ? '*' : '/';
        consume(m_current.type);
        ASTPtr right = parseFactor();
        node = std::make_unique<BinaryOpNode>(op, std::move(node), std::move(right));
    }
    return node;
}

ASTPtr Parser::parseFactor() {
    // Factor -> NUMBER | LPAREN Expression RPAREN
    if (m_current.type == Token::NUMBER) {
        double val = m_current.value;
        consume(Token::NUMBER);
        return std::make_unique<NumberNode>(val);
    } else if (m_current.type == Token::LPAREN) {
        consume(Token::LPAREN);
        ASTPtr node = parseExpression();
        consume(Token::RPAREN);
        return node;
    } else {
        throw std::runtime_error("Expected number or '('");
    }
}
