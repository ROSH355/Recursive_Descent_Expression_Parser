#include "parser.h"
#include <stdexcept>
#include <sstream>

Parser::Parser(Lexer lex, SymbolTable symbols) 
    : m_lexer(std::move(lex)), m_symbols(std::move(symbols)) {
    m_current = m_lexer.peek();
}

void Parser::advanceToken() {
    m_lexer.get();
    m_current = m_lexer.peek();
}

void Parser::consume(Token::Type type) {
    if (m_current.type == type) {
        advanceToken();
    } else {
        std::ostringstream oss;
        oss << "Syntax error at position " << m_current.pos 
            << ": unexpected token";
        throw std::runtime_error(oss.str());
    }
}

ASTPtr Parser::parseStatement() {
    // Statement -> IDENTIFIER ASSIGN Expression | Expression
    // or EOF returns nullptr
    
    if (m_current.type == Token::END) {
        return nullptr;
    }
    
    // Try to detect assignment: save position and parse an expression
    // If it's just a variable and next is ASSIGN, handle as assignment
    size_t saved_pos = m_current.pos;
    
    if (m_current.type == Token::IDENTIFIER) {
        std::string var_name = m_current.text;
        Token peek_token = m_current;
        advanceToken();
        
        if (m_current.type == Token::ASSIGN) {
            // This is an assignment
            consume(Token::ASSIGN);
            ASTPtr expr = parseExpression();
            expr = expr->optimize();
            
            // Evaluate and store in symbol table
            double value = expr->evaluate(m_symbols);
            m_symbols.set(var_name, value);
            
            return expr; // Return the expression for the result
        } else {
            // Not an assignment; the identifier is part of the expression
            // Continue parsing from where we are (m_current is past the identifier)
            ASTPtr var_node = std::make_unique<VariableNode>(var_name);
            
            // Handle term operations (* /)
            while (m_current.type == Token::MUL || m_current.type == Token::DIV) {
                char op = (m_current.type == Token::MUL) ? '*' : '/';
                consume(m_current.type);
                ASTPtr right = parseFactor();
                var_node = std::make_unique<BinaryOpNode>(op, std::move(var_node), std::move(right));
            }
            
            // Handle expression operations (+ -)
            while (m_current.type == Token::PLUS || m_current.type == Token::MINUS) {
                char op = (m_current.type == Token::PLUS) ? '+' : '-';
                consume(m_current.type);
                ASTPtr right = parseTerm();
                var_node = std::make_unique<BinaryOpNode>(op, std::move(var_node), std::move(right));
            }
            
            var_node = var_node->optimize();
            return var_node;
        }
    }
    
    // Not an identifier; parse as normal expression
    return parseExpression();
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
    // Factor -> NUMBER
    //         | IDENTIFIER
    //         | LPAREN Expression RPAREN
    //         | MINUS Factor
    //         | PLUS Factor
    
    if (m_current.type == Token::NUMBER) {
        double val = m_current.value;
        consume(Token::NUMBER);
        return std::make_unique<NumberNode>(val);
    } 
    else if (m_current.type == Token::IDENTIFIER) {
        std::string name = m_current.text;
        consume(Token::IDENTIFIER);
        return std::make_unique<VariableNode>(name);
    }
    else if (m_current.type == Token::LPAREN) {
        consume(Token::LPAREN);
        ASTPtr node = parseExpression();
        consume(Token::RPAREN);
        return node;
    }
    else if (m_current.type == Token::MINUS) {
        consume(Token::MINUS);
        ASTPtr operand = parseFactor();
        return std::make_unique<UnaryOpNode>('-', std::move(operand));
    }
    else if (m_current.type == Token::PLUS) {
        consume(Token::PLUS);
        ASTPtr operand = parseFactor();
        return std::make_unique<UnaryOpNode>('+', std::move(operand));
    }
    else {
        throw std::runtime_error("Expected number, identifier, or '('");
    }
}

SymbolTable &Parser::getSymbols() {
    return m_symbols;
}
