//
// Created by Funcieq on 27.11.2025.
//

#ifndef IGNIS_PARSER_H
#define IGNIS_PARSER_H
#include <vector>
#include "../token/token.h"
#include "../ast/node.h"

class Parser {
    public:
    std::vector<Token> tokens;
    unsigned long pos;
    std::vector<Node> Parse(std::vector<Token> *tokens);
    Token Peek();
    Token Advance();
    Token Previous();
    bool IsAtEnd();
    Token Consume(TokenType expectedType, std::string errMsg);
    Node ParseStatement();
    Node ParseFunction();
    Node ParseParams();
};

#endif //IGNIS_PARSER_H