//
// Created by Funcieq on 28.11.2025.
//

#include "parser.h"

#include <pthread.h>
#include <stdexcept>

std::vector<Node> Parser::Parse(std::vector<Token>* tokensInput) {

    tokens = *tokensInput;
    pos = 0;

    std::vector<Node> nodes;


    while (!IsAtEnd()) {
        nodes.push_back(ParseStatement());
    }

    return nodes;
}

Node Parser::ParseStatement() {
    if (Peek().type == TokenType::KFn) {
        return ParseFunction();
    }
    else {
        throw std::runtime_error("statement of type: "+ std::string(Peek().value)+" is not implemented yet");
    }
}

Node Parser::ParseFunction() {
    Advance();
    std::string_view funcName = Peek().value;
    Consume(TokenType::LParen, "Expected '('");
}

Token Parser::Peek() {
    if (IsAtEnd()) {
        return tokens.back();
    }
    return tokens[pos];
}

Token Parser::Advance() {
    if (!IsAtEnd()) pos++;
    return Previous();
}

Token Parser::Previous() {
    return tokens[pos - 1];
}

bool Parser::IsAtEnd() {
    return pos >= tokens.size() || tokens[pos].type == TokenType::EOF_TOKEN;
}

Token Parser::Consume(TokenType expectedType, std::string errMsg) {
    if (Peek().type == expectedType) {
        return Advance();
    }
    throw std::runtime_error(
        errMsg + " Found: " + std::string(Peek().value)
    );
}
