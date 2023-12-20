#ifndef ZOMBIE_H
#define ZOMBIE_H

#include "denizen.h"

using namespace simulation;

class Zombie : public Denizen
{
private:

public:
    Zombie(int id, int districtId, State initialState) : Denizen(id, districtId, initialState) {}
    // virtual ~Zombie() {}

    void setState(State newState) override {}

    State getState() const override { return State::Zombie; }
};

#endif