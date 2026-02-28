#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <string>

int main() {
    std::cout << "Recursive Descent Parser with Variables & Unary Operators\n";
    std::cout << "Enter expressions or assignments (one per line, empty line to finish):\n";
    std::cout << "Example: x = 5, y = x * 3, y + 2\n\n";
    
    try {
        SymbolTable symbols;
        std::string line;
        
        while (true) {
            std::cout << "> ";
            if (!std::getline(std::cin, line)) break;
            
            if (line.empty()) break;
            
            try {
                Lexer lexer(line);
                Parser parser(std::move(lexer), symbols);
                auto ast = parser.parseStatement();
                
                if (!ast) {
                    std::cout << "Empty statement.\n";
                    continue;
                }
                
                // Optimize the AST (constant folding, etc.)
                auto optimized = ast->optimize();
                
                std::cout << "AST Structure:\n";
                optimized->print();
                
                double result = optimized->evaluate(symbols);
                std::cout << "Result: " << result << "\n\n";
                
                // Update symbols for next iteration
                symbols = parser.getSymbols();
                
            } catch (const std::exception &e) {
                std::cerr << "Error: " << e.what() << "\n\n";
            }
        }
        
        std::cout << "\nDone!\n";
        
    } catch (const std::exception &e) {
        std::cerr << "Fatal error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
