#pragma once

#include <iostream>
#include <string>
#include <unordered_map>

enum class TokenType {
    // Single character tokens
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    PLUS,
    MINUS,
    ASTERISK,
    SLASH,
    SEMICOLON,

    // Single or double character tokens
    NOT,
    EQUAL,
    NOT_EQUAL,
    EQUAL_EQUAL,
    GREATER,
    GREATER_EQUAL,
    LESSER,
    LESSER_EQUAL,

    // Literals
    IDENTIFIER,
    STRING,
    NUMBER,

    // Keywords
    AND,
    OR,
    TRUE,
    FALSE,
    IF,
    ELSE,
    FOR,
    WHILE,
    LET,
    NIL,
    FUNCTION,
    RETURN,

};

class Token {
public:
    TokenType type;
    std::string lexeme;
    std::string value;
    int line;

    Token(TokenType _type, std::string _lexeme, std::string _value, int _line)
        : type(_type), lexeme(_lexeme), value(_value), line(_line) {}
};

// Debug helper
std::ostream &operator<<(std::ostream &out, const Token &token) {
    out << token.lexeme << std::endl;
    return out;
}

static std::unordered_map<std::string, TokenType> keywords = {
    {"and", TokenType::AND},
    {"or", TokenType::OR},
    {"true", TokenType::TRUE},
    {"false", TokenType::FALSE},
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"for", TokenType::FOR},
    {"while", TokenType::WHILE},
    {"let", TokenType::LET},
    {"nil", TokenType::NIL},
    {"function", TokenType::FUNCTION},
    {"return", TokenType::RETURN}
};