#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>
#include <algorithm>

enum class TokenType {
    RETURN,
    INT_LITERAL,
    SEMI
};

class Token {
public:
    TokenType type;
    std::optional<std::string> value;

    Token() : type(TokenType::SEMI) {}
    Token(TokenType type) : type(type) {}
    Token(TokenType type, std::string value) : type(type), value(value) {}
};

std::vector<Token> tokenize(const std::string &source) {
    std::vector<Token> tokens;

    std::stringstream buffer;

    for (int i = 0; i < source.size(); i++) {
        while (std::isalnum(source.at(i))) {
            buffer << source.at(i);
            i++;
        }
        // i--;

        if (std::isspace(source.at(i))) {
            if (buffer.str() == "return")
                tokens.push_back(Token(TokenType::RETURN));
            buffer.str("");
            buffer.clear();
            continue;
        }

        if (source.at(i) == ';') {
            std::string s = buffer.str().c_str();
            if (!s.empty() && std::all_of(s.begin(), s.end(), ::isdigit))
                tokens.push_back(Token(TokenType::INT_LITERAL, buffer.str()));
            buffer.str("");
            buffer.clear();
            tokens.push_back(Token(TokenType::SEMI));
        }
    }

    return tokens;
}

std::string genASM(const std::vector<Token> tokens) {
    std::stringstream buffer;
    buffer << "global _start\n_start:\n";

    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i].type == TokenType::RETURN) {
            buffer << "\tmov eax, 60\n";
            i++;
            buffer << "\tmov edi, " << tokens[i].value.value() << "\n";
        } else if (tokens[i].type == TokenType::SEMI) {
            buffer << "\tsyscall\n";
        }
    }

    return buffer.str();
}

int main(int argc, char **argv) {
    // Check if the user has provided a file to compile
    if (argc < 2) {
        std::cerr << "Usage\n\tradiance <source>\n\nSpecify a file for radiance to compilel.\n";
        exit(EXIT_FAILURE);
    }

    // Open the file and read its contents
    std::ifstream file(argv[1]);
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();

    // Tokenize the contents
    std::vector<Token> tokens = tokenize(buffer.str());

    // Write the ASM to a file
    std::string instructions = genASM(tokens);
    {
        std::ofstream file("main.asm");
        file << instructions.c_str();
        file.close();
    }

    // Compile assembly
    system("nasm -felf64 main.asm");
    system("ld main.o -o main");

    exit(EXIT_SUCCESS);
}