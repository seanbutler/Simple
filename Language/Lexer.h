//
// Created by sean on 16/06/2020.
//

#ifndef GLYNTH_LEXER_H
#define GLYNTH_LEXER_H

#include "Tokens.h"

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <tuple>

class Lexer {

public:

    Lexer()=default;

    void SetSourceString(const std::string &S, unsigned int P = 0){
        lexingStr = S;
        lexingPos = P;
    }

    virtual ~Lexer() {
    }

    Token ScanToken();

    Token Scan() {
        Token result;
        do {
            result = ScanToken();
        } while (result != Token::END_OF_FILE && result != Token::ERROR);

        return result;
    }

    std::vector<std::pair<Token, std::string>> GetTokenList() {
        return tokens;
    }

    std::vector<std::tuple<Token, std::string, int>> GetTokenListWithLines() {
        return tokensWithLine;
    }

    void OutputTokenList(std::string filename) {
        std::ofstream tokenListFile;
        std::string tokenListFilename = std::filesystem::path(filename);
        tokenListFilename += std::string(".tokens");
        tokenListFile.open(tokenListFilename);
        for (auto it = tokens.begin(); it != tokens.end(); ++it) {
            tokenListFile << (int) it->first << "\t" << it->second << std::endl;
        }
        tokenListFile.close();
    }

    void OutputTokenListWithLines(std::string filename) {
        std::ofstream tokenListFile;
        std::string tokenListFilename = std::filesystem::path(filename);
        tokenListFilename += std::string(".tokAndLine");
        tokenListFile.open(tokenListFilename);
//        for (auto it = tokensWithLine.begin(); it != tokensWithLine.end(); ++it) {
        for (int i = 0; i < tokensWithLine.size(); i++) {
            tokenListFile
                    << (int) std::get<0>(tokensWithLine[i])
                    << "\t"
                    << std::get<1>(tokensWithLine[i])
                    << "\t"
                    << (int) std::get<2>(tokensWithLine[i])
                    << std::endl;
        }
        tokenListFile.close();
    }

private:
    std::string lexingStr;
    unsigned int lexingPos = 0;
    unsigned int currentLine = 1;
    std::vector<std::pair<Token, std::string>> tokens;
    std::vector<std::tuple<Token, std::string, int>> tokensWithLine;
};


#endif //GLYNTH_LEXER_H
