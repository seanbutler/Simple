//
// Created by sean on 10/07/2020.
//

#ifndef GLYNTH_INSTRUCTIONS_H
#define GLYNTH_INSTRUCTIONS_H

#include <string>
#include <array>

enum class INS : int {
    NOP = 0,
    HALT,
    YIELD,
    PUSH,
    POP,

    SAVE,
    LOAD,

    SAVEALIEN,
    LOADALIEN,

    JMP,
    BRT,
    BRF,

    ADD,
    SUB,
    MUL,
    DIV,

    EQU,
    NE,
    GT,
    GTE,
    LT,
    LTE,

    AND,
    OR,
    NOT,

    OUTPUT,
    INPUT,

    // these take 1 parameter and dont return, but have side effects in the alien vars
    MOVE,
    TURN,

    // these take no parameters but returns a value
    RAND,

    // these take one parameter and return one parameter
    SENSE,


    NUMBER,
    ADDRESS,
    LABEL,

    END_OF_FILE,
    ERROR
};

extern char* instructionNames[];

#endif //GLYNTH_INSTRUCTIONS_H
