#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include <unordered_map>

// Forward declaration
class SymbolTable;

// Base class for AST nodes.
class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual double evaluate(const SymbolTable &symbols) const = 0;
    virtual void print(int indent = 0) const = 0;
    virtual std::unique_ptr<ASTNode> optimize() = 0;
};

using ASTPtr = std::unique_ptr<ASTNode>;

// Simple symbol table for variable storage
class SymbolTable {
public:
    void set(const std::string &name, double value);
    double get(const std::string &name) const;
    bool has(const std::string &name) const;

private:
    std::unordered_map<std::string, double> m_vars;
};

// Represents a numeric literal.
class NumberNode : public ASTNode {
public:
    explicit NumberNode(double value);
    double evaluate(const SymbolTable &symbols) const override;
    void print(int indent = 0) const override;
    std::unique_ptr<ASTNode> optimize() override;

private:
    double m_value;
};

// Represents a variable lookup.
class VariableNode : public ASTNode {
public:
    explicit VariableNode(const std::string &name);
    double evaluate(const SymbolTable &symbols) const override;
    void print(int indent = 0) const override;
    std::unique_ptr<ASTNode> optimize() override;

private:
    std::string m_name;
};

// Represents a unary operation (-, +).
class UnaryOpNode : public ASTNode {
public:
    UnaryOpNode(char op, ASTPtr operand);
    double evaluate(const SymbolTable &symbols) const override;
    void print(int indent = 0) const override;
    std::unique_ptr<ASTNode> optimize() override;

private:
    char m_op;
    ASTPtr m_operand;
};

// Represents a binary operation (+, -, *, /).
class BinaryOpNode : public ASTNode {
public:
    BinaryOpNode(char op, ASTPtr left, ASTPtr right);
    double evaluate(const SymbolTable &symbols) const override;
    void print(int indent = 0) const override;
    std::unique_ptr<ASTNode> optimize() override;

private:
    char m_op;
    ASTPtr m_left;
    ASTPtr m_right;
};

#endif // AST_H
