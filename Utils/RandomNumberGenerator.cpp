#include "RandomNumberGenerator.h"

std::default_random_engine util::RandomNumberGenerator::engine = std::default_random_engine(std::random_device{}());

int util::RandomNumberGenerator::RandNum(int min, int max)
{
    std::uniform_int_distribution dist(min, max);
    return dist(engine);
}