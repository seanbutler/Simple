//
// Created by sean on 16/06/2020.
//

#include "Lexer.h"

Token Lexer::ScanToken() {
    char currentChar = lexingStr[lexingPos++];

    SCANSTART:

    //
    // skip whitespace
    //
    while (currentChar == ' ' || currentChar == '\t' || currentChar == '\n') {
        if (currentChar == '\n') {
            currentLine++;
        }
        currentChar = lexingStr[lexingPos++];
    }

    //
    // skip comment
    //
    if (currentChar == '#') {
        do{
            currentChar = lexingStr[lexingPos++];
        } while (currentChar != '\n');
        currentLine++;
        currentChar = lexingStr[lexingPos++];
        goto SCANSTART;
    }

    //
    // token, probably keyword maybe identifier
    //
    if (isalpha(currentChar) ) {

        // starting a new token ... probably a keyword
        std::string tokenString;

        // get the next character
        tokenString += currentChar;
        currentChar = lexingStr[lexingPos++];

        // scan forward letters, numbers and underscores only
        while (isalnum(currentChar) || currentChar == '_') {
            tokenString += currentChar;
            currentChar = lexingStr[lexingPos++];
        }
        --lexingPos;
        if (tokenString == "if") {
            tokens.push_back(std::make_pair(Token::KEYWORD_IF, tokenString));
            tokensWithLine.push_back(std::make_tuple(Token::KEYWORD_IF, tokenString, currentLine));
            return Token::KEYWORD_IF;
        } else if (tokenString == "while") {
            tokens.push_back(std::make_pair(Token::KEYWORD_WHILE, tokenString));
            tokensWithLine.push_back(std::make_tuple(Token::KEYWORD_WHILE, tokenString, currentLine));
            return Token::KEYWORD_WHILE;
        } else if (tokenString == "var") {
            tokens.push_back(std::make_pair(Token::KEYWORD_VAR, tokenString));
            tokensWithLine.push_back(std::make_tuple(Token::KEYWORD_VAR, tokenString, currentLine));
            return Token::KEYWORD_VAR;
//        } else if (tokenString == "func") {
//            tokens.push_back(std::make_pair(Token::KEYWORD_FUNC, tokenString));
//            return Token::KEYWORD_FUNC;
//        } else if (tokenString == "return") {
//            tokens.push_back(std::make_pair(Token::KEYWORD_RETURN, tokenString));
//            return Token::KEYWORD_RETURN;
        } else if (tokenString == "yield") {
            tokens.push_back(std::make_pair(Token::KEYWORD_YIELD, tokenString));
            tokensWithLine.push_back(std::make_tuple(Token::KEYWORD_YIELD, tokenString, currentLine));
            return Token::KEYWORD_YIELD;
        } else if (tokenString == "exit") {
            tokens.push_back(std::make_pair(Token::KEYWORD_EXIT, tokenString));
            tokensWithLine.push_back(std::make_tuple(Token::KEYWORD_EXIT, tokenString, currentLine));
            return Token::KEYWORD_EXIT;
        } else if (tokenString == "output") {
            // TODO make this an alien variable rather than a special command?
            tokens.push_back(std::make_pair(Token::KEYWORD_OUTPUT, tokenString));
            tokensWithLine.push_back(std::make_tuple(Token::KEYWORD_OUTPUT, tokenString, currentLine));
            return Token::KEYWORD_OUTPUT;
        }
        else if (tokenString == "move") {
            tokens.push_back(std::make_pair(Token::KEYWORD_MOVE, tokenString));
            tokensWithLine.push_back(std::make_tuple(Token::KEYWORD_MOVE, tokenString, currentLine));

            return Token::KEYWORD_MOVE;
        }
        else if (tokenString == "turn") {
            tokens.push_back(std::make_pair(Token::KEYWORD_TURN, tokenString));
            tokensWithLine.push_back(std::make_tuple(Token::KEYWORD_TURN, tokenString, currentLine));

            return Token::KEYWORD_TURN;
        }
        else if (tokenString == "rand") {
            tokens.push_back(std::make_pair(Token::KEYWORD_RANDFUN, tokenString));
            tokensWithLine.push_back(std::make_tuple(Token::KEYWORD_RANDFUN, tokenString, currentLine));

            return Token::KEYWORD_RANDFUN;
        }
        else if (tokenString == "sense") {
            tokens.push_back(std::make_pair(Token::KEYWORD_SENSEFUN, tokenString));
            tokensWithLine.push_back(std::make_tuple(Token::KEYWORD_SENSEFUN, tokenString, currentLine));

            return Token::KEYWORD_SENSEFUN;
        }
        else if (tokenString == "stick") {
            tokens.push_back(std::make_pair(Token::KEYWORD_STICKFUN, tokenString));
            tokensWithLine.push_back(std::make_tuple(Token::KEYWORD_STICKFUN, tokenString, currentLine));

            return Token::KEYWORD_STICKFUN;
        }
        else if (tokenString == "direction") {
            tokens.push_back(std::make_pair(Token::KEYWORD_DIRECTIONFUN, tokenString));
            tokensWithLine.push_back(std::make_tuple(Token::KEYWORD_DIRECTIONFUN, tokenString, currentLine));

            return Token::KEYWORD_DIRECTIONFUN;
        }
        else if (tokenString == "keyboard") {
            tokens.push_back(std::make_pair(Token::KEYWORD_KEYBOARDFUN, tokenString));
            tokensWithLine.push_back(std::make_tuple(Token::KEYWORD_KEYBOARDFUN, tokenString, currentLine));

            return Token::KEYWORD_KEYBOARDFUN;
        }
        else if (tokenString == "buttons") {
            tokens.push_back(std::make_pair(Token::KEYWORD_BUTTONSFUN, tokenString));
            tokensWithLine.push_back(std::make_tuple(Token::KEYWORD_BUTTONSFUN, tokenString, currentLine));

            return Token::KEYWORD_BUTTONSFUN;
        }

        tokens.push_back(std::make_pair(Token::IDENTIFIER, tokenString));
        tokensWithLine.push_back(std::make_tuple(Token::IDENTIFIER, tokenString, currentLine));

        return Token::IDENTIFIER;
    }

    //
    // alien variable (interface with the simulation, like posx, posy, r, g, b etc)
    //
//    if ( currentChar == '$' ) {
//
//        // starting a new token... probably a keyword
//        std::string tokenString;
//
//        // get the next character
//        tokenString += currentChar;
//        currentChar = lexingStr[lexingPos++];
//
//        // scan forward letters only
//        while (isalpha(currentChar) ) {
//            tokenString += currentChar;
//            currentChar = lexingStr[lexingPos++];
//        }
//        --lexingPos;
//
//        tokens.push_back(std::make_pair(Token::IDENTIFIER, tokenString));
//        return Token::IDENTIFIER;
//    }


    //
    // numbers
    //
    // def: start with [0-9.], follow by [0-9.]
    // number only has 'double' type
    if (isdigit(currentChar) || currentChar == '.') {
        std::string numberStr;
        numberStr += currentChar;
        currentChar = lexingStr[lexingPos++];
        while (isdigit(currentChar) || currentChar == '.') {
            numberStr += currentChar;
            currentChar = lexingStr[lexingPos++];
        }
        --lexingPos;
//        tokens.push_back(std::make_pair(Token::NUMBER, numberStr));
        tokensWithLine.push_back(std::make_tuple(Token::NUMBER, numberStr, currentLine));
        return Token::NUMBER;
    }

    //
    // other symbols...
    //
    switch (currentChar) {

        case '\0': {
            tokens.push_back(std::make_pair(Token::END_OF_FILE, "EOF"));
            tokensWithLine.push_back(std::make_tuple(Token::END_OF_FILE, "EOF", currentLine));

            return Token::END_OF_FILE;
        }

        case '{': {
            tokens.push_back(std::make_pair(Token::SYM_LBRACES, "{"));
            tokensWithLine.push_back(std::make_tuple(Token::SYM_LBRACES, "{", currentLine));
            return Token::SYM_LBRACES;
        }

        case '}': {
            tokens.push_back(std::make_pair(Token::SYM_RBRACES, "}"));
            tokensWithLine.push_back(std::make_tuple(Token::SYM_RBRACES, "}", currentLine));

            return Token::SYM_RBRACES;
        }

        case '(': {
            tokens.push_back(std::make_pair(Token::SYM_LPAREN, "("));
            tokensWithLine.push_back(std::make_tuple(Token::SYM_LPAREN, "(", currentLine));

            return Token::SYM_LPAREN;
        }

        case ')': {
            tokens.push_back(std::make_pair(Token::SYM_RPAREN, ")"));
            tokensWithLine.push_back(std::make_tuple(Token::SYM_RPAREN, ")", currentLine));

            return Token::SYM_RPAREN;
        }

        case '[': {
            tokens.push_back(std::make_pair(Token::SYM_LBRACKET, "["));
            tokensWithLine.push_back(std::make_tuple(Token::SYM_LBRACKET, "[", currentLine));
            return Token::SYM_LBRACKET;
        }

        case ']': {
            tokens.push_back(std::make_pair(Token::SYM_RBRACKET, "]"));
            tokensWithLine.push_back(std::make_tuple(Token::SYM_RBRACKET, "]", currentLine));

            return Token::SYM_RBRACKET;
        }

        case '>': {
            tokens.push_back(std::make_pair(Token::OP_GT, ">"));
            tokensWithLine.push_back(std::make_tuple(Token::OP_GT, ">", currentLine));

            return Token::OP_GT;
        }

        case '<': {
            tokens.push_back(std::make_pair(Token::OP_LT, "<"));
            tokensWithLine.push_back(std::make_tuple(Token::OP_LT, "<", currentLine));

            return Token::OP_LT;
        }

        case '+': {
            tokens.push_back(std::make_pair(Token::OP_ADD, "+"));
            tokensWithLine.push_back(std::make_tuple(Token::OP_ADD, "+", currentLine));

            return Token::OP_ADD;
        }

        case '-': {
            tokens.push_back(std::make_pair(Token::OP_SUB, "-"));
            tokensWithLine.push_back(std::make_tuple(Token::OP_SUB, "-", currentLine));

            return Token::OP_SUB;
        }

        case '*': {
            tokens.push_back(std::make_pair(Token::OP_MUL, "*"));
            tokensWithLine.push_back(std::make_tuple(Token::OP_MUL, "*", currentLine));

            return Token::OP_MUL;
        }

        case '/': {

            tokens.push_back(std::make_pair(Token::OP_DIV, "/"));
            tokensWithLine.push_back(std::make_tuple(Token::OP_DIV, "/", currentLine));

            return Token::OP_DIV;
        }

        case '&': {
            char nextChar = lexingStr[lexingPos];

            if (nextChar == '&') {
                tokens.push_back(std::make_pair(Token::OP_AND, "&&"));
                tokensWithLine.push_back(std::make_tuple(Token::OP_AND, "&&", currentLine));

                ++lexingPos;
                return Token::OP_AND;
            }

            std::cerr << "TOKENIZER ERROR after \'&\' at Line " << currentLine << " Got " << currentChar << std::endl;
            return Token::ERROR;
        }

        case '|': {
            char nextChar = lexingStr[lexingPos];

            if (nextChar == '|') {
                tokens.push_back(std::make_pair(Token::OP_OR, "||"));
                tokensWithLine.push_back(std::make_tuple(Token::OP_OR, "||", currentLine));

                ++lexingPos;
                return Token::OP_OR;
            }

            std::cerr << "TOKENIZER ERROR after \'|\' at Line " << currentLine << " Got " << currentChar << std::endl;
            return Token::ERROR;
        }

        case '!': {
            char nextChar = lexingStr[lexingPos];

            if (nextChar == '=') {
                tokens.push_back(std::make_pair(Token::OP_NE, "!="));
                tokensWithLine.push_back(std::make_tuple(Token::OP_NE, "!=", currentLine));

                ++lexingPos;
                return Token::OP_NE;
            }
            std::cerr << "TOKENIZER ERROR after \'!\' at Line " << currentLine << " Got " << currentChar << std::endl;
            return Token::ERROR;
        }

        case '=': {
            char nextChar = lexingStr[lexingPos];

            if (nextChar == '=') {
                tokens.push_back(std::make_pair(Token::OP_EQ, "=="));
                tokensWithLine.push_back(std::make_tuple(Token::OP_EQ, "==", currentLine));

                ++lexingPos;
                return Token::OP_EQ;
            }
            tokens.push_back(std::make_pair(Token::SYM_ASSIGN, "="));
            tokensWithLine.push_back(std::make_tuple(Token::SYM_ASSIGN, "=", currentLine));

            return Token::SYM_ASSIGN;
        }

        case ';': {
            tokens.push_back(std::make_pair(Token::SYM_SEMICOLON, ";"));
            tokensWithLine.push_back(std::make_tuple(Token::SYM_SEMICOLON, ";", currentLine));

            return Token::SYM_SEMICOLON;
        }

        case ':': {
            tokens.push_back(std::make_pair(Token::SYM_COLON, ":"));
            tokensWithLine.push_back(std::make_tuple(Token::SYM_COLON, ":", currentLine));

            return Token::SYM_COLON;
        }

//        case '#': {
//            tokens.push_back(std::make_pair(Token::SYM_HASH, "#"));
//            return Token::SYM_HASH;
//        }

        default: {
            std::cerr << "TOKENIZER WARNING Line (" << currentLine << ") Possibly '" << currentChar << "'" << std::endl;
            return Token::ERROR;
        }
    }
}