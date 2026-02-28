# Recursive Descent Expression Parser with AST Visualization

This project implements a basic arithmetic expression parser using the recursive descent technique. It tokenizes input, builds an abstract syntax tree (AST), prints the tree structure, and evaluates the expression.

## 🌳 Grammar Rules

```
Expression -> Term ((PLUS | MINUS) Term)*
Term       -> Factor ((MUL | DIV) Factor)*
Factor     -> NUMBER | LPAREN Expression RPAREN
```

Operators supported: `+`, `-`, `*`, `/` with proper precedence and parentheses.

## 🛠️ AST Design

- **ASTNode** (base class)
  - `evaluate()` – compute value
  - `print(int indent)` – pretty-print tree
- **NumberNode** – leaf node holding a numeric literal
- **BinaryOpNode** – internal node representing `+ - * /`

Unique pointers (`std::unique_ptr`) manage node lifetime. No global state.

## 🚧 Error Handling

- Syntax errors produce exceptions with descriptive messages.
- Division by zero is detected during evaluation and reported.

## 🧪 Example Input / Output

```
Enter an arithmetic expression: (3 + 4) * 5

AST Structure:
BinaryOp(*)
  BinaryOp(+)
    Number(3)
    Number(4)
  Number(5)

Result: 35
```

## ⚙️ Compilation Instructions

Make sure you have a C++17-compliant compiler (e.g. `g++`).

```sh
cd RecursiveDescentParser

g++ -std=c++17 *.cpp -o parser

./parser
```

Type an expression when prompted and press Enter.

## 🔮 Future Scope

- Support variables and assignment
- Add unary operators (`+`/`-`)
- Extend to functions or more complex grammars
- Generate code or bytecode from AST

This project is structured and documented with clarity, making it suitable for inclusion in a compiler/systems internship portfolio.
