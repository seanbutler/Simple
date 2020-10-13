//
// Created by sean on 27/07/2020.
//

#ifndef GLYNTH_SPECIALVARIABLES_H
#define GLYNTH_SPECIALVARIABLES_H

// setting and getting these are exposed to the VMs
// used to get values out of the VM and into the simulation where necessary

class SpecialVariables {
public:
    SpecialVariables() { }

    std::map<std::string, int>  dict;
};

class AlienVariables : public SpecialVariables {
public:
    AlienVariables() {}
};

class GlobalVariables : public SpecialVariables {
public:
    GlobalVariables() {}

};

#endif //GLYNTH_SPECIALVARIABLES_H
