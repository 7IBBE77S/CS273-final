#include "utility.h"
#include <random>

namespace utility
{

    std::random_device rd;
    std::mt19937 gen(rd());
    double generateRandomDouble(double min, double max)
    {
        std::uniform_real_distribution<> dis(min, max);
        return dis(gen);
    }

    bool checkProbability(double probability)
    {
        return generateRandomDouble(0.0, 1.0) < probability;
    }

}
