#include "lexer.h"
#include <cctype>
#include <stdexcept>

Lexer::Lexer(const std::string &input)
    : m_input(input), m_pos(0) {
    m_current = getNextToken();
}

const Token &Lexer::peek() const {
    return m_current;
}

Token Lexer::get() {
    Token t = m_current;
    m_current = getNextToken();
    return t;
}

void Lexer::skipWhitespace() {
    while (m_pos < m_input.size() && std::isspace(static_cast<unsigned char>(m_input[m_pos]))) {
        ++m_pos;
    }
}

Token Lexer::getNextToken() {
    skipWhitespace();
    if (m_pos >= m_input.size())
        return Token(Token::END);

    char ch = m_input[m_pos];
    if (std::isdigit(ch) || ch == '.') {
        // parse number
        size_t start = m_pos;
        bool dotSeen = (ch == '.');
        ++m_pos;
        while (m_pos < m_input.size()) {
            char c = m_input[m_pos];
            if (std::isdigit(c)) {
                ++m_pos;
            } else if (c == '.' && !dotSeen) {
                dotSeen = true;
                ++m_pos;
            } else {
                break;
            }
        }
        double value = std::stod(m_input.substr(start, m_pos - start));
        return Token(Token::NUMBER, value);
    }

    ++m_pos;
    switch (ch) {
        case '+': return Token(Token::PLUS);
        case '-': return Token(Token::MINUS);
        case '*': return Token(Token::MUL);
        case '/': return Token(Token::DIV);
        case '(':
            return Token(Token::LPAREN);
        case ')':
            return Token(Token::RPAREN);
        default:
            throw std::runtime_error(std::string("Unknown character: ") + ch);
    }
}
