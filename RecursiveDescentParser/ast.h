#ifndef AST_H
#define AST_H

#include <memory>

// Base class for AST nodes.
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual double evaluate() const = 0;
    virtual void print(int indent = 0) const = 0;
};

using ASTPtr = std::unique_ptr<ASTNode>;

// Represents a numeric literal.
class NumberNode : public ASTNode {
public:
    explicit NumberNode(double value);
    double evaluate() const override;
    void print(int indent = 0) const override;

private:
    double m_value;
};

// Represents a binary operation (+, -, *, /).
class BinaryOpNode : public ASTNode {
public:
    BinaryOpNode(char op, ASTPtr left, ASTPtr right);
    double evaluate() const override;
    void print(int indent = 0) const override;

private:
    char m_op;
    ASTPtr m_left;
    ASTPtr m_right;
};

#endif // AST_H
