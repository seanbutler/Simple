#ifndef GLYNTH_RANDOMNUMBERGENERATOR_H
#define GLYNTH_RANDOMNUMBERGENERATOR_H

#include <random>

namespace util
{
    class RandomNumberGenerator {
    public:
        static int RandNum(int min, int max);
    private:
        static std::default_random_engine engine;
    };
}




#endif //GLYNTH_RANDOMNUMBERGENERATOR_H
