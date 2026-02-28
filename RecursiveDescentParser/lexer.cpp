#include "lexer.h"
#include <cctype>
#include <stdexcept>
#include <sstream>

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
        return Token(Token::END, 0.0, m_pos);

    char ch = m_input[m_pos];
    size_t start_pos = m_pos;
    
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
        return Token(Token::NUMBER, value, start_pos);
    }
    
    if (std::isalpha(ch) || ch == '_') {
        // parse identifier
        size_t start = m_pos;
        while (m_pos < m_input.size() && (std::isalnum(static_cast<unsigned char>(m_input[m_pos])) || m_input[m_pos] == '_')) {
            ++m_pos;
        }
        Token t(Token::IDENTIFIER, 0.0, start_pos);
        t.text = m_input.substr(start, m_pos - start);
        return t;
    }

    ++m_pos;
    switch (ch) {
        case '+': return Token(Token::PLUS, 0.0, start_pos);
        case '-': return Token(Token::MINUS, 0.0, start_pos);
        case '*': return Token(Token::MUL, 0.0, start_pos);
        case '/': return Token(Token::DIV, 0.0, start_pos);
        case '(': return Token(Token::LPAREN, 0.0, start_pos);
        case ')': return Token(Token::RPAREN, 0.0, start_pos);
        case '=': return Token(Token::ASSIGN, 0.0, start_pos);
        default:
            throw std::runtime_error(std::string("Unknown character at position ") + std::to_string(start_pos) + ": " + ch);
    }
}
