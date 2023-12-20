#ifndef ALARMED_H
#define ALARMED_H

#include "denizen.h"
#include "zombie.h"

using namespace simulation;

class Alarmed : public Denizen
{
private:

enum State currentState;

public:
    Alarmed(int id, int districtId, State initialState) : Denizen(id, districtId, initialState) {}
    // virtual ~Alarmed() {}

     void setState(State newState) override {
        if (newState == State::Zombie) {
            currentState = newState;
        }
    }

    State getState() const override { return State::Alarmed; }
};

#endif