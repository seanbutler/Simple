//
// Created by sean on 23/07/2020.
//

#ifndef GLYNTH_ALIENVARS_H
#define GLYNTH_ALIENVARS_H

#include <array>
#include <functional>

// ----------------------------------------------------------------------

class AlienVars{

    // TODO - change this so we can access the arrays via string index rather than numbers

public:
    void set(unsigned int var, int val)             { values[var] = val;}
    int get(unsigned int var)                       { return values[var];}
    int call(int F, int P)                              { return actions[F](P);}

    std::array<int, 32> values;
    std::array <std::function<int(int)>, 32> actions;

};

// ----------------------------------------------------------------------

#endif //GLYNTH_ALIENVARS_H
