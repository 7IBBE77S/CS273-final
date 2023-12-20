#ifndef UTILITIES_H
#define UTILITIES_H

#include <random>

namespace utility
{

    // change as needed
    constexpr double IGNORANT_TO_ZOMBIE_PROB = 0.000;  // X
    constexpr double IGNORANT_TO_ALARMED_PROB = 0.000; // Y
    constexpr double ALARMED_TO_ZOMBIE_PROB = 0.0000;   // Z
    static constexpr double ZOMBIE_BITE_SUCCESS_PROB = 0.0;
    static constexpr double IGNORANT_MOVE_PROB = 0.00;
    static constexpr double ALARMED_MOVE_PROB = 0.0;
    static constexpr double ZOMBIE_MOVE_PROB = 0.0;
    constexpr int DOWNTOWN = 3;
    constexpr int MEDICAL_HILL = 1;
    constexpr int THE_BURBS = 0;
    constexpr int UNIVERSITY = 2;
    constexpr int PATIENT_ZERO_ID = 0;
    constexpr int ALARMED_DENIZEN_ID = 0;
    constexpr int NUM_DISTRICTS = 4;
    constexpr int NUM_DENIZENS = 2000;
    constexpr int NUM_DAYS = 28;

    double generateRandomDouble(double min, double max);
    bool checkProbability(double probability);

}

#endif 
       // bernoulli distribution or binomial distribution?