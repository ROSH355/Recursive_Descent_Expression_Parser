#include "ast.h"
#include <iostream>
#include <stdexcept>
#include <string>

NumberNode::NumberNode(double value) : m_value(value) {}

double NumberNode::evaluate() const {
    return m_value;
}

void NumberNode::print(int indent) const {
    for (int i = 0; i < indent; ++i) std::cout << ' ';
    std::cout << "Number(" << m_value << ")\n";
}

BinaryOpNode::BinaryOpNode(char op, ASTPtr left, ASTPtr right)
    : m_op(op), m_left(std::move(left)), m_right(std::move(right)) {}

double BinaryOpNode::evaluate() const {
    double l = m_left->evaluate();
    double r = m_right->evaluate();
    switch (m_op) {
        case '+': return l + r;
        case '-': return l - r;
        case '*': return l * r;
        case '/':
            if (r == 0.0)
                throw std::runtime_error("Division by zero");
            return l / r;
        default:
            throw std::runtime_error(std::string("Unknown operator ") + m_op);
    }
}

void BinaryOpNode::print(int indent) const {
    for (int i = 0; i < indent; ++i) std::cout << ' ';
    std::cout << "BinaryOp(" << m_op << ")\n";
    m_left->print(indent + 2);
    m_right->print(indent + 2);
}
