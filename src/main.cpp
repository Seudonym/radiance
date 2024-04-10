#include <fstream>
#include <sstream>
#include "Scanner.hpp"


int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: radiance [text]";
        exit(EXIT_FAILURE);
    }
    
    std::string source;
    {
        std::stringstream buffer;
        std::ifstream file(argv[1]);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open file " << argv[1];
            exit(EXIT_FAILURE);
        }

        buffer << file.rdbuf();
        source = buffer.str();
        file.close();        
    }

    std::cout << "\033[1;32m" << "Source:" << std::endl << "\033[0m";
    std::cout << source << std::endl << std::endl;

    Scanner scanner(source);
    std::vector<Token> tokens = scanner.scanTokens();

    exit(EXIT_SUCCESS);
}