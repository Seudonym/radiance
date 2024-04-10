#pragma once

#include <vector>
#include "Token.hpp"

class Scanner {
public:
    Scanner(const std::string &_source) : source(_source) {
        start = current = 0;
        line = 1;
    }

    void scanToken();
    std::vector<Token> scanTokens();

private:
    std::string source;
    std::vector<Token> tokens;
    int start, current, line;

    // Traverse
    bool atEnd() const;
    char peek(int offset);
    char advance();
    bool matchNext(char expected);

    // Helpers
    void addToken(TokenType type);
    void addToken(TokenType type, const std::string &value);

    // Literal handlers
    void handleString();
    void handleNumber();
    void handleIdentifier();
};

// Public
std::vector<Token> Scanner::scanTokens() {
    while (!atEnd()) {
        start = current;
        scanToken();
    }

    return tokens;
}

// Private
bool Scanner::atEnd() const {
    return current >= source.length();
}

char Scanner::advance() {
    return source.at(current++);
}

char Scanner::peek(int offset = 0) {
    // Peek ahead from the next character by 'offset' amount
    current += offset;
    char result = atEnd() ? '\0' : source.at(current);
    current -= offset;
    return result;
}

bool Scanner::matchNext(char expected) {
    if (atEnd())
        return false;
    if (source.at(current) != expected)
        return false;
    current++;
    return true;
}

void Scanner::addToken(TokenType type) {
    addToken(type, "");
}

void Scanner::addToken(TokenType type, const std::string &value) {
    std::string lexeme = source.substr(start, current - start);
    tokens.push_back(Token(type, lexeme, value, line));
}

void Scanner::handleString() {
    // Consume till "
    while (peek() != '"' && !atEnd()) {
        if (peek() == '\n')
            line++;
        advance();
    }

    if (atEnd()) {
        std::cerr << "Unterminated string";
        return;
    }

    // Consume the last "
    advance();

    // Remove trailing and ending " for the value
    std::string value = source.substr(start + 1, current - start - 2);
    addToken(TokenType::STRING, value);
}

void Scanner::handleNumber() {
    // Consume digits till a . is found or digits end
    while (std::isdigit(peek())) advance();
    if (peek() == '.') {
        advance();
        while (std::isdigit(peek())) advance();
    }
    addToken(TokenType::NUMBER, source.substr(start, current - start));
}

void Scanner::handleIdentifier() {
    // Consume letter, digit and underscore
    while (std::isalnum(peek()) || peek() == '_') advance();
    
    std::string lexeme = source.substr(start, current - start);
    // If a keyword exists, then the token type should be the keyword's token type. Else it is an identifier
    TokenType type = (keywords.find(lexeme) != keywords.end()) ? keywords.at(lexeme) : TokenType::IDENTIFIER;

    addToken(type);
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
    // Single character
    case '(':
        addToken(TokenType::LEFT_PAREN);
        break;
    case ')':
        addToken(TokenType::RIGHT_PAREN);
        break;
    case '{':
        addToken(TokenType::LEFT_BRACE);
        break;
    case '}':
        addToken(TokenType::RIGHT_BRACE);
        break;
    case ',':
        addToken(TokenType::COMMA);
        break;
    case '.':
        addToken(TokenType::DOT);
        break;
    case '+':
        addToken(TokenType::PLUS);
        break;
    case '-':
        addToken(TokenType::MINUS);
        break;
    case '*':
        addToken(TokenType::ASTERISK);
        break;
    case ';':
        addToken(TokenType::SEMICOLON);
        break;

    // Single or double character
    case '!':
        addToken(matchNext('=') ? TokenType::NOT_EQUAL : TokenType::NOT);
        break;
    case '=':
        addToken(matchNext('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
        break;
    case '>':
        addToken(matchNext('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
        break;
    case '<':
        addToken(matchNext('=') ? TokenType::LESSER_EQUAL : TokenType::LESSER);
        break;
    case '/':
        if (matchNext('/'))
            while (peek() != '\n' && !atEnd())
                advance();
        else
            addToken(TokenType::SLASH);
        break;

    // Whitespaces
    case ' ':
    case '\r':
    case '\t':
        break;

    case '\n':
        line++;
        break;

    // Literals
    case '"':
        handleString();
        break;

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        handleNumber();
        break;

    default:
        if (std::isalpha(c)) {
            handleIdentifier();
            break;
        }
        std::cerr << "Unexpected token";
        break;
    }
}