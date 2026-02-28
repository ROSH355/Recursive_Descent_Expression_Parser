#include "lexer.h"
#include "parser.h"
#include <iostream>
#include <string>
#include <sstream>

// Helper function to capture AST output to a string
std::string captureASTOutput(const std::unique_ptr<ASTNode>& ast) {
    std::stringstream ss;
    std::streambuf* old_cout = std::cout.rdbuf(ss.rdbuf());
    ast->print();
    std::cout.rdbuf(old_cout);
    return ss.str();
}

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
                
                // Capture original AST before optimization
                std::string original_ast_output = captureASTOutput(ast);
                
                // Optimize the AST (constant folding, etc.)
                auto optimized = ast->optimize();
                
                // Capture optimized AST
                std::string optimized_ast_output = captureASTOutput(optimized);
                
                // Print original AST
                std::cout << "Original AST:\n" << original_ast_output;
                
                // Check if optimization changed the AST
                if (original_ast_output == optimized_ast_output) {
                    std::cout << "No optimization applied.\n\n";
                } else {
                    std::cout << "Optimized AST:\n" << optimized_ast_output << "\n";
                }
                
                // Evaluate and print result
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
