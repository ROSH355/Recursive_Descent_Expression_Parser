#include "ast.h"
#include <iostream>
#include <stdexcept>
#include <string>
#include <cmath>

// SymbolTable implementation
void SymbolTable::set(const std::string &name, double value) {
    m_vars[name] = value;
}

double SymbolTable::get(const std::string &name) const {
    auto it = m_vars.find(name);
    if (it == m_vars.end()) {
        throw std::runtime_error("Undefined variable: " + name);
    }
    return it->second;
}

bool SymbolTable::has(const std::string &name) const {
    return m_vars.find(name) != m_vars.end();
}

// NumberNode implementation
NumberNode::NumberNode(double value) : m_value(value) {}

double NumberNode::evaluate(const SymbolTable &symbols) const {
    (void)symbols; // unused
    return m_value;
}

void NumberNode::print(int indent) const {
    for (int i = 0; i < indent; ++i) std::cout << ' ';
    std::cout << "Number(" << m_value << ")\n";
}

std::unique_ptr<ASTNode> NumberNode::optimize() {
    return std::make_unique<NumberNode>(m_value);
}

// VariableNode implementation
VariableNode::VariableNode(const std::string &name) : m_name(name) {}

double VariableNode::evaluate(const SymbolTable &symbols) const {
    return symbols.get(m_name);
}

void VariableNode::print(int indent) const {
    for (int i = 0; i < indent; ++i) std::cout << ' ';
    std::cout << "Variable(" << m_name << ")\n";
}

std::unique_ptr<ASTNode> VariableNode::optimize() {
    return std::make_unique<VariableNode>(m_name);
}

// UnaryOpNode implementation
UnaryOpNode::UnaryOpNode(char op, ASTPtr operand)
    : m_op(op), m_operand(std::move(operand)) {}

double UnaryOpNode::evaluate(const SymbolTable &symbols) const {
    double val = m_operand->evaluate(symbols);
    switch (m_op) {
        case '-': return -val;
        case '+': return val;
        default:
            throw std::runtime_error(std::string("Unknown unary operator: ") + m_op);
    }
}

void UnaryOpNode::print(int indent) const {
    for (int i = 0; i < indent; ++i) std::cout << ' ';
    std::cout << "UnaryOp(" << m_op << ")\n";
    m_operand->print(indent + 2);
}

std::unique_ptr<ASTNode> UnaryOpNode::optimize() {
    auto optimized_operand = m_operand->optimize();
    
    // Constant folding for unary operators
    if (auto *num = dynamic_cast<NumberNode *>(optimized_operand.get())) {
        SymbolTable empty_table;
        double val = num->evaluate(empty_table);
        switch (m_op) {
            case '-': return std::make_unique<NumberNode>(-val);
            case '+': return std::make_unique<NumberNode>(val);
            default:
                return std::make_unique<UnaryOpNode>(m_op, std::move(optimized_operand));
        }
    }
    return std::make_unique<UnaryOpNode>(m_op, std::move(optimized_operand));
}

// BinaryOpNode implementation
BinaryOpNode::BinaryOpNode(char op, ASTPtr left, ASTPtr right)
    : m_op(op), m_left(std::move(left)), m_right(std::move(right)) {}

double BinaryOpNode::evaluate(const SymbolTable &symbols) const {
    double l = m_left->evaluate(symbols);
    double r = m_right->evaluate(symbols);
    switch (m_op) {
        case '+': return l + r;
        case '-': return l - r;
        case '*': return l * r;
        case '/':
            if (r == 0.0)
                throw std::runtime_error("Division by zero");
            return l / r;
        default:
            throw std::runtime_error(std::string("Unknown operator: ") + m_op);
    }
}

void BinaryOpNode::print(int indent) const {
    for (int i = 0; i < indent; ++i) std::cout << ' ';
    std::cout << "BinaryOp(" << m_op << ")\n";
    m_left->print(indent + 2);
    m_right->print(indent + 2);
}

std::unique_ptr<ASTNode> BinaryOpNode::optimize() {
    auto opt_left = m_left->optimize();
    auto opt_right = m_right->optimize();
    
    // Constant folding: if both children are numbers, compute result
    auto *left_num = dynamic_cast<NumberNode *>(opt_left.get());
    auto *right_num = dynamic_cast<NumberNode *>(opt_right.get());
    
    if (left_num && right_num) {
        SymbolTable empty_table;
        double result = 0.0;
        switch (m_op) {
            case '+': result = left_num->evaluate(empty_table) + right_num->evaluate(empty_table); break;
            case '-': result = left_num->evaluate(empty_table) - right_num->evaluate(empty_table); break;
            case '*': result = left_num->evaluate(empty_table) * right_num->evaluate(empty_table); break;
            case '/':
                if (right_num->evaluate(empty_table) == 0.0)
                    throw std::runtime_error("Division by zero in constant folding");
                result = left_num->evaluate(empty_table) / right_num->evaluate(empty_table);
                break;
            default:
                return std::make_unique<BinaryOpNode>(m_op, std::move(opt_left), std::move(opt_right));
        }
        return std::make_unique<NumberNode>(result);
    }
    
    return std::make_unique<BinaryOpNode>(m_op, std::move(opt_left), std::move(opt_right));
}
