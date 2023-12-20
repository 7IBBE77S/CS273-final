#ifndef IGNORANT_H
#define IGNORANT_H

#include "denizen.h"
#include "alarmed.h"
#include "zombie.h"


using namespace simulation;

class Ignorant : public Denizen
{
private:

enum State currentState;


public:
    Ignorant(int id, int districtId, State initialState) : Denizen(id, districtId, initialState) {}
    // virtual ~Ignorant() {}

    void setState(State newState) override {
        if (newState == State::Alarmed || newState == State::Zombie) {
            currentState = newState;
        }
    }

    State getState() const override { return State::Ignorant; }
};

#endif