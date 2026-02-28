# Recursive Descent Expression Parser with AST Visualization

A professional-grade arithmetic expression parser featuring unary operators, variable assignments, constant folding optimization, and clear AST visualization. Built in C++17 with clean OOP design patterns. Ideal for compiler design coursework and portfolio projects.

## 🌳 Updated Grammar

```
Statement -> IDENTIFIER ASSIGN Expression
           | Expression
Expression -> Term ((PLUS | MINUS) Term)*
Term       -> Factor ((MUL | DIV) Factor)*
Factor     -> (MINUS | PLUS) Factor
           | PRIMARY
PRIMARY    -> NUMBER
           | IDENTIFIER
           | LPAREN Expression RPAREN
```

**Operators supported**: `+`, `-`, `*`, `/` with proper precedence and associativity.
**Unary operators**: Unary minus (`-`) and unary plus (`+`) with right associativity.

## 🆕 New Features (5 Enhancements)

### Feature 1: Unary Operator Support
- Unary minus and plus fully integrated into the grammar
- Right-associative (e.g., `--5` evaluates correctly)
- **AST Node**: `UnaryOpNode` for unary operations
- **Example**: Input `-5 + 3` produces:
  ```
  BinaryOp(+)
    UnaryOp(-)
      Number(5)
    Number(3)
  Result: -2
  ```

### Feature 2: Symbol Table & Variable Assignment
- Full variable support with persistent symbol table
- Assignment syntax: `variable = expression`
- Variables recall values across statements
- Runtime error on undefined variable usage
- **AST Node**: `VariableNode` for variable references
- **Example**:
  ```
  > x = 5
  Result: 5
  > y = x * 3
  Result: 15
  > y + 2
  Result: 17
  ```

### Feature 3: Constant Folding Optimization
- AST optimization pass reduces binary operations with `NumberNode` children
- Optimizations applied recursively throughout the tree
- Compilation-like behavior without actual code generation
- **Method**: `ASTNode::optimize()` on all node types
- **Example**: `3 + 4` becomes a single `Number(7)` node

### Feature 4: Enhanced Error Handling
- Descriptive syntax errors with input position information
- Division-by-zero detection during evaluation
- Clear runtime messages for undefined variables
- Token position tracked for precise error reporting
- **Error Format**: `Syntax error at position X: description`

### Feature 5: AST Pretty Printing
- Tree-style indentation for visual clarity
- Each node type clearly labeled (`Number`, `Variable`, `UnaryOp`, `BinaryOp`)
- Recursive descent prints in depth-first order
- **Indentation**: 2 spaces per level

---

## 🛠️ AST Design

### Class Hierarchy

```cpp
ASTNode (abstract base)
├── NumberNode         (numeric literals)
├── VariableNode       (variable references)
├── UnaryOpNode        (unary -, +)
└── BinaryOpNode       (binary +, -, *, /)

SymbolTable           (variable storage)
```

### Base Class Interface

```cpp
class ASTNode {
    virtual double evaluate(const SymbolTable &symbols) const = 0;
    virtual void print(int indent = 0) const = 0;
    virtual std::unique_ptr<ASTNode> optimize() = 0;
};
```

### Memory Management
- Smart pointers (`std::unique_ptr`) throughout
- No raw pointer leaks
- Automatic RAII cleanup

---

## 🧪 Example Session

```
Enter expressions or assignments (one per line, empty line to finish):
> x = 5
AST Structure:
Number(5)
Result: 5

> y = x * 3
AST Structure:
BinaryOp(*)
  Variable(x)
  Number(3)
Result: 15

> -(y + 2)
AST Structure:
UnaryOp(-)
  BinaryOp(+)
    Variable(y)
    Number(2)
Result: -17

> 3 + 4
AST Structure:
Number(7)
Result: 7
```

## ⚙️ Compilation

Requires C++17 compiler (e.g., `g++ 7+`, `clang++ 6+`).

```sh
cd RecursiveDescentParser
g++ -std=c++17 *.cpp -o parser
./parser
```

Interactive mode: type one expression/assignment per line, press Enter twice to exit.

---

## 📋 Project Files

| File | Purpose |
|----|----|
| `main.cpp` | Multi-statement REPL with symbol table persistence |
| `lexer.h/.cpp` | Tokenization (identifiers, numbers, operators, assignment) |
| `parser.h/.cpp` | Recursive descent parser with unary and assignment support |
| `ast.h/.cpp` | AST node definitions and optimize/evaluate implementations |
| `README.md` | Documentation |

---

## 🔮 Future Enhancements

- **Function definitions**: `def f(x) = x * 2; f(5)`
- **More operators**: `%` (modulo), `^` (exponentiation), `**` (power)
- **Built-in functions**: `sin`, `cos`, `sqrt`, `abs`
- **Graphviz export**: `--dot` flag to generate `.dot` files for visualization
- **REPL improvements**: `?` help, `:clear` to reset variables, history
- **Code generation**: Compile AST to bytecode or x86 assembly
- **Debugger**: Step through evaluation, print symbol table state

---

## 💡 Learning Outcomes

This project demonstrates:
- **Recursive descent parsing** technique
- **AST** design and traversal
- **Symbol tables** for variable storage
- **Optimization passes** (constant folding)
- **Error handling** in parsers
- **C++ best practices** (smart pointers, inheritance, virtual methods)
- **Clean code separation** (lexer/parser/AST modules)

Perfect preparation for compiler design courses or systems internships!
