//
// Created by sean on 09/07/2020.
//

#include "metainstructions.h"

#include "Assembler.h"
#include <string>
#include <algorithm>
#include <iostream>

INS Assembler::ScanInstruction() {

    char currentChar = assemblyStr[assemblyPos++];

    START_OF_SCAN:

    //
    // skip whitespace
    //
    while (currentChar == ' ' || currentChar == '\t' || currentChar == '\n') {
        if (currentChar == '\n') {
            currentLine++;
        }
        currentChar = assemblyStr[assemblyPos++];
        goto START_OF_SCAN;
    }

    //
    // comments begin with a semicolon (#) skip them
    //
    if (currentChar == COMMENT[0]) {
        do {
            currentChar = assemblyStr[assemblyPos++];
        } while (currentChar != '\n');
        currentLine++;
        goto START_OF_SCAN;
    }

    //
    // assembly instructions
    //
    if (isalpha(currentChar)) {

        // starting a new token ...
        std::string tokenString;

        // get the next character
        tokenString += currentChar;
        currentChar = assemblyStr[assemblyPos++];

        // scan forward letters and underscore
        while (isalnum(currentChar) || currentChar == '_') {
            tokenString += currentChar;
            currentChar = assemblyStr[assemblyPos++];
        }
        --assemblyPos;

//        std::cout << "asm got op/lab\t\t" << tokenString << std::endl;

        // TODO - turn this whole logic into a lookup or polymorphism

        if (tokenString.compare("NOP") == 0) {
            this->instructions.push_back((int) INS::NOP);
            return INS::NOP;
        }

        if (tokenString.compare("HALT") == 0) {
            this->instructions.push_back((int) INS::HALT);
            return INS::HALT;
        }

        if (tokenString.compare("YIELD") == 0) {
            this->instructions.push_back((int) INS::YIELD);
            return INS::YIELD;
        }

        if (tokenString.compare("SAVE") == 0) {
            this->instructions.push_back((int) INS::SAVE);
            return INS::SAVE;
        }

        if (tokenString.compare("LOAD") == 0) {
            this->instructions.push_back((int) INS::LOAD);
            return INS::LOAD;
        }

//        if (tokenString.compare("SAVEALIEN") == 0) {
//            this->instructions.push_back((int) INS::SAVEALIEN);
//            return INS::SAVEALIEN;
//        }
//
//        if (tokenString.compare("LOADALIEN") == 0) {
//            this->instructions.push_back((int) INS::LOADALIEN);
//            return INS::LOADALIEN;
//        }

        if (tokenString.compare("PUSH") == 0) {
            this->instructions.push_back((int) INS::PUSH);
            return INS::PUSH;
        }

        if (tokenString.compare("POP") == 0) {
            this->instructions.push_back((int) INS::POP);
            return INS::POP;
        }

        if (tokenString.compare("ADD") == 0) {
            this->instructions.push_back((int) INS::ADD);
            return INS::ADD;
        }

        if (tokenString.compare("SUB") == 0) {
            this->instructions.push_back((int) INS::SUB);
            return INS::SUB;
        }

        if (tokenString.compare("MUL") == 0) {
            this->instructions.push_back((int) INS::MUL);
            return INS::MUL;
        }
        if (tokenString.compare("DIV") == 0) {
            this->instructions.push_back((int) INS::DIV);
            return INS::DIV;
        }
        if (tokenString.compare("AND") == 0) {
            this->instructions.push_back((int) INS::AND);
            return INS::AND;
        }
        if (tokenString.compare("OR") == 0) {
            this->instructions.push_back((int) INS::OR);
            return INS::OR;
        }
        if (tokenString.compare("EQU") == 0) {
            this->instructions.push_back((int) INS::EQU);
            return INS::EQU;
        }
        if (tokenString.compare("NE") == 0) {
            this->instructions.push_back((int) INS::NE);
            return INS::NE;
        }
        if (tokenString.compare("GT") == 0) {
            this->instructions.push_back((int) INS::GT);
            return INS::GT;
        }
        if (tokenString.compare("GTE") == 0) {
            this->instructions.push_back((int) INS::GTE);
            return INS::GTE;
        }
        if (tokenString.compare("LT") == 0) {
            this->instructions.push_back((int) INS::LT);
            return INS::LT;
        }
        if (tokenString.compare("LTE") == 0) {
            this->instructions.push_back((int) INS::LTE);
            return INS::LTE;
        }

        if (tokenString.compare("RAND") == 0) {
            this->instructions.push_back((int) INS::RAND);
            return INS::RAND;
        }

        if (tokenString.compare("SENSE") == 0) {
            this->instructions.push_back((int) INS::SENSE);
            return INS::SENSE;
        }

        if (tokenString.compare("OUTPUT") == 0) {
            this->instructions.push_back((int) INS::OUTPUT);
            return INS::OUTPUT;
        }
        if (tokenString.compare("INPUT") == 0) {
            this->instructions.push_back((int) INS::INPUT);
            return INS::INPUT;
        }

        if (tokenString.compare("MOVE") == 0) {
            this->instructions.push_back((int) INS::MOVE);
            return INS::MOVE;
        }
//        if (tokenString.compare("TURN") == 0) {
//            this->instructions.push_back((int) INS::TURN);
//            return INS::TURN;
//        }

        if (tokenString.compare("JMP") == 0) {
            this->instructions.push_back((int) INS::JMP);
            return INS::JMP;
        }
        if (tokenString.compare("BRT") == 0) {
            this->instructions.push_back((int) INS::BRT);
            return INS::BRT;
        }
        if (tokenString.compare("BRF") == 0) {
            this->instructions.push_back((int) INS::BRF);
            return INS::BRF;
        }

    }

    //
    // numbers
    //
    // def: start with [0-9], follow by [0-9.]
    // number currently turns into a int in the VM
    if (isdigit(currentChar)) {

        std::string numberStr = "";
        numberStr += currentChar;
        currentChar = assemblyStr[assemblyPos++];
        while (isdigit(currentChar) || currentChar == '.') {
            numberStr += currentChar;
            currentChar = assemblyStr[assemblyPos++];
        }

        --assemblyPos;

        this->instructions.push_back(std::stoi(numberStr));
        return INS::NUMBER;
    }

    //
    // %LABELS - they indicate data addresses
    // add them to a map and insert their memory address here)
    //
    if (currentChar == VARIABLE[0]) {

        // starting a new token ...
        std::string tokenString;

        // starting from the next character so we lose the LOCATION PREFIX
        currentChar = assemblyStr[assemblyPos++];

        // scan forward letters and underscore
        while (isalnum(currentChar) || currentChar == '_') {
            tokenString += currentChar;
            currentChar = assemblyStr[assemblyPos++];
        }
        --assemblyPos;

//        std::cout << "asm got "<< VARIABLE << " label\t\t" << tokenString << std::endl;

        std::pair<std::map<std::string,int>::iterator,bool> ret;

        ret = data.insert(std::make_pair(tokenString, data.size()));
        if ( ret.second ) {
//            std::cout << "element " << tokenString << " inserted at address " << ret.first->second << '\n';
        } else  {
//            std::cout << "element " << tokenString << " already existed at address " << ret.first->second << '\n';
        }
        this->instructions.push_back(ret.first->second);

        return INS::LABEL;
    }

    //
    // :LABELS (they indicate jump destinations, no instructions emitted,
    //          instead add these addresses to a map with the label string)
    //
    if (currentChar == LOCATION[0]) {

        // starting a new token ...
        std::string tokenString;

        // starting from the next character so we lose the LOCATION PREFIX
        currentChar = assemblyStr[assemblyPos++];

        // scan forward letters and underscore
        while (isalnum(currentChar) || currentChar == '_') {
            tokenString += currentChar;
            currentChar = assemblyStr[assemblyPos++];
        }
        --assemblyPos;

//        std::cout << "asm got : label\t\t" << tokenString << std::endl;

        labels.insert(std::make_pair(tokenString, this->instructions.size()));
        return INS::LABEL;
    }

    //
    // @LABELS addresses to be jumped to, emit a NOP (or NaN) for now and patch them up later
    //
    if (currentChar == DESTINATION[0]) {

        // starting a new token ... probably an instruction address
        std::string tokenString;

        // starting from the next character so we lose the DESTINATION PREFIX
        currentChar = assemblyStr[assemblyPos++];

        // scan forward letters, numbers and underscores only
        while (isalnum(currentChar) || currentChar == '_') {
            tokenString += currentChar;
            currentChar = assemblyStr[assemblyPos++];
        }
        --assemblyPos;

        // add it to a map indexed on the string
//        std::cout << "asm got @ label\t\t" << tokenString << "\n";

        // reference to a location in the instructions
        jumps.insert(std::make_pair(this->instructions.size(), tokenString));
        this->instructions.push_back((int) -999999);
        return INS::LABEL;
    }

    //
    // other symbols...
    //
    if (currentChar == '\0') {
        instructions.push_back((int) INS::END_OF_FILE);
        return INS::END_OF_FILE;
    }
}