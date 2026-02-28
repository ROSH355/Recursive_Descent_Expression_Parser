#ifndef LEXER_H
#define LEXER_H

#include <string>

// A simple token class for arithmetic expressions.
struct Token {
    enum Type {
        NUMBER,
        PLUS,
        MINUS,
        MUL,
        DIV,
        LPAREN,
        RPAREN,
        IDENTIFIER,
        ASSIGN,
        END
    } type;
    double value; // valid only when type == NUMBER
    std::string text; // valid for IDENTIFIER
    size_t pos; // position in input

    Token(Type t = END, double v = 0.0, size_t p = 0) : type(t), value(v), pos(p) {}
};

class Lexer {
public:
    explicit Lexer(const std::string &input);
    // Returns the current token without consuming it.
    const Token &peek() const;
    // Consume and return the current token, advancing to the next one.
    Token get();

private:
    void skipWhitespace();
    Token getNextToken();

    std::string m_input;
    size_t m_pos;
    Token m_current;
};

#endif // LEXER_H
