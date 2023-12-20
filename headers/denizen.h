#ifndef DENIZEN_H
#define DENIZEN_H

#include <vector>
#include "graph.h"

namespace simulation
{

    class Denizen
    {
    public:
        enum class State
        {
            Ignorant,
            Alarmed,
            Zombie
        };                                                  // possible states of a denizen
        Denizen(int id, int districtId, State initialState) // constructor
            : id(id), districtId(districtId), currentState(initialState)
        {
        }

        virtual ~Denizen() = default; // destructor

        int getId() const { return id; }                 // return unique id
        int getDistrictId() const { return districtId; } // return district (location) id
        // State getState() const { return currentState; }  // return state

        void setDistrictId(int newDistrictId) { districtId = newDistrictId; } // change district (location) id

        // const version of setDistrictId
        // void setDistrictId(int newDistrictId, const Graph &graph) {
        //     if (graph.areAdjacent(districtId, newDistrictId)) {
        //         districtId = newDistrictId;
        //     }
        // }
        // void setState(State newState) { currentState = newState; } // change state

        //virtual version of setState
        virtual void setState(State newState) { currentState = newState; } // change state



        // virtual getState
        virtual State getState() const = 0;

        // update state based on interaction (to be implemented)
        // void updateState(const std::vector<Denizen>& otherDenizens);  // used in update function in district to pass in stats of individual denizens

        // move to a different district
        // void moveTo(int newDistrictId, const Graph &graph);

    private:
        int id;             // unique identifier for the denizen maybe make atomic nextid and increment
        int districtId;     // current district ID where the denizen is located
        State currentState; // current state (Ignorant, Alarmed, Zombie)
    };

}

#endif
