#include "district.h"
#include "denizen.h"
#include "graph.h"
#include "zombie.h"
#include "alarmed.h"
#include "ignorant.h"
#include "utility.h"
#include <iostream>

namespace simulation
{
    // Constructor for District class
    District::District(int id, const std::string &name) : id(id), name(name) {}

    // Destructor for District class
    District::~District()
    {
        // Delete all denizens in the district
        for (auto denizen : denizens)
        {
            delete denizen; // Make sure this is the only place denizens are deleted
        }
    }

    // Function to create a new denizen with a new state
    template <typename OldState, typename NewState>
    NewState *District::createNewDenizen(OldState *oldDenizen)
    {
        // Create a new denizen with the same ID and district ID as the old denizen, but with the new state
        NewState *newDenizen = new NewState(oldDenizen->getId(), oldDenizen->getDistrictId(), oldDenizen->getState());
        return newDenizen;
    }

    // Function to add a denizen to the district
    void District::addDenizen(Denizen *denizen)
    {
        // Add the denizen to the denizens vector
        denizens.push_back(denizen);
        // Increment the count for the denizen's state
        stateCounts[denizen->getState()]++;
    }

    // Function to remove a denizen from the district
    void District::removeDenizen(int denizenId)
    {
        // Iterate over the denizens vector
        for (auto it = denizens.begin(); it != denizens.end(); it++)
        {
            // If the denizen's ID matches the given ID
            if ((*it)->getId() == denizenId)
            {
                // Decrement the count for the denizen's state
                stateCounts[(*it)->getState()]--;
                // Remove the denizen from the denizens vector
                denizens.erase(it);
                return;
            }
        }
    }

    // Function to update a denizen's state
    Denizen *District::updateState(Denizen *&denizen)
    {
        // Get the current state of the denizen
        Denizen::State currentState = denizen->getState();

        // Check the current state of the denizen
        switch (currentState)
        {
        case Ignorant::State::Ignorant:
            // If the denizen is ignorant, check if it should become alarmed or a zombie
            for (auto &other : denizens)
            {
                if (denizen->getId() != other->getId())
                {
                    if (other->getState() == Zombie::State::Zombie && utility::checkProbability(utility::IGNORANT_TO_ZOMBIE_PROB))
                    {
                        return createNewDenizen<Ignorant, Zombie>(static_cast<Ignorant *>(denizen));
                    }
                    else if (other->getState() == Alarmed::State::Alarmed && utility::checkProbability(utility::IGNORANT_TO_ALARMED_PROB))
                    {
                        return createNewDenizen<Ignorant, Alarmed>(static_cast<Ignorant *>(denizen));
                    }
                }
            }
            break;
        case Alarmed::State::Alarmed:
            // If the denizen is alarmed, check if it should become a zombie
            for (auto &other : denizens)
            {
                if (denizen->getId() != other->getId())
                {
                    if (other->getState() == Zombie::State::Zombie && utility::checkProbability(utility::ALARMED_TO_ZOMBIE_PROB))
                    {
                        return createNewDenizen<Alarmed, Zombie>(static_cast<Alarmed *>(denizen));
                    }
                }
            }
            break;
        case Zombie::State::Zombie:
            // Zombies don't change state
            break;
        }
        return nullptr; // No state change required
    }

    // Function to update all denizens in the district
    void District::update(const Graph &graph)
    {
        // Create a copy of the denizens vector
        std::vector<Denizen *> denizensCopy = denizens;

        // Iterate over the copy of the denizens vector
        for (auto it = denizensCopy.begin(); it != denizensCopy.end(); ++it)
        {
            // Get the old state of the denizen
            Denizen::State oldState = (*it)->getState();

            // Update the state of the denizen
            Denizen *newDenizen = updateState(*it);

            // If the denizen's state was changed
            if (newDenizen != nullptr)
            {
                // Add the new denizen to the denizens vector
                addDenizen(newDenizen);
                // Remove the old denizen from the denizens vector
                removeDenizen((*it)->getId());
                // Delete the old denizen
                delete *it;
            }
            // If the denizen's state was not changed
            else if ((*it)->getState() != oldState)
            {
                // Update the state counts
                stateCounts[oldState]--;
                stateCounts[(*it)->getState()]++;
            }
        }
    }

    // Function to determine if a denizen should move
    bool District::shouldMove(const Denizen &denizen)
    {
        // Check the denizen's state and return the appropriate probability
        switch (denizen.getState())
        {
        case Ignorant::State::Ignorant:
            return utility::checkProbability(utility::IGNORANT_MOVE_PROB);
        case Alarmed::State::Alarmed:
            return utility::checkProbability(utility::ALARMED_MOVE_PROB);
        case Zombie::State::Zombie:
            return utility::checkProbability(utility::ZOMBIE_MOVE_PROB);
        }
        return false; // Default case if no other conditions are met
    }

    // Function to determine the new district ID for a denizen that is moving
    int District::determineNewDistrictId(const Denizen &denizen, const Graph &graph)
    {
        // Get the IDs of the adjacent districts
        auto adjacentDistricts = graph.getNeighbors(id);

        // If there are adjacent districts
        if (!adjacentDistricts.empty())
        {
            // Select a random adjacent district
            int randomIndex = static_cast<int>(utility::generateRandomDouble(0, adjacentDistricts.size() - 1));
            return adjacentDistricts[randomIndex];
        }
        else
        {
            // If there are no adjacent districts, stay in the same district
            return id;
        }
    }

}