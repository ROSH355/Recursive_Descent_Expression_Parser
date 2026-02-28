#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <string>

int main() {
    std::cout << "Enter an arithmetic expression: ";
    std::string line;
    if (!std::getline(std::cin, line))
        return 0;

    try {
        Lexer lexer(line);
        Parser parser(std::move(lexer));
        auto ast = parser.parse();

        std::cout << "\nAST Structure:\n";
        ast->print();

        double result = ast->evaluate();
        std::cout << "\nResult: " << result << "\n";
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
