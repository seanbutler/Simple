//
// Created by sean on 22/07/2020.
//

#ifndef GLYNTH_METAINSTRUCTIONS_H
#define GLYNTH_METAINSTRUCTIONS_H

//
// These symbols are used in the asm to indicate different types and context
// for symbmols, especially identifiers for jumps and labels. also comments.
//

#include <string>

const std::string COMMENT = "#";
const std::string VARIABLE = "%";
const std::string LOCATION = ":";
const std::string DESTINATION = "@";

#endif //GLYNTH_METAINSTRUCTIONS_H
