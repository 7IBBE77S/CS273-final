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

    void District::moveTo(Denizen *denizen, const Graph &graph, const std::vector<District *> &districts)
    {
        if (denizen->getState() == Denizen::State::Ignorant)
        {
            // Find the safest adjacent district for Ignorant denizens
            int minThreat = INT_MAX;
            int targetDistrictId = this->id;
            for (int neighborId : graph.getNeighbors(this->id))
            {
                int zombieCount = districts[neighborId]->getStateCounts()[Denizen::State::Zombie];
                int alarmedCount = districts[neighborId]->getStateCounts()[Denizen::State::Alarmed];
                int threatLevel = zombieCount + alarmedCount; // Combine counts to assess threat

                if (threatLevel < minThreat)
                {
                    minThreat = threatLevel;
                    targetDistrictId = neighborId;
                }
            }

            if (targetDistrictId != this->id)
            {
                denizen->setDistrictId(targetDistrictId);
            }
        }
        else
        {
            // Use the existing logic for other states
            int newDistrictId = determineNewDistrictId(*denizen, graph);
            if (graph.areAdjacent(id, newDistrictId))
            {
                denizen->setDistrictId(newDistrictId);
            }
        }
    }

  void District::moveFromZombies(Denizen* denizen, const Graph& graph, const std::vector<District*>& districts) {
    if (utility::checkProbability(utility::ALARMED_MOVE_PROB)) {
        // Initialize variables to track the best district to move to
        int minZombies = INT_MAX;
        int maxIgnorants = 0;
        int targetDistrictId = this->id;

        // Check neighboring districts
        for (int neighborId : graph.getNeighbors(this->id)) {
            auto neighborStateCounts = districts[neighborId]->getStateCounts();
            int zombieCount = neighborStateCounts[Denizen::State::Zombie];
            int ignorantCount = neighborStateCounts[Denizen::State::Ignorant];

            // Prioritize districts with fewer zombies and more ignorants
            if (zombieCount < minZombies || (zombieCount == minZombies && ignorantCount > maxIgnorants)) {
                minZombies = zombieCount;
                maxIgnorants = ignorantCount;
                targetDistrictId = neighborId;
            }
        }

        // Move to the selected district
        if (targetDistrictId != this->id) {
            denizen->setDistrictId(targetDistrictId);
        }
    } else {
        // Random movement if not specifically avoiding zombies or seeking ignorants
        moveTo(denizen, graph, districts);
    }
}


    void District::moveToDenizens(Denizen *denizen, const Graph &graph, const std::vector<District *> &districts)
    {
        if (utility::checkProbability(utility::ZOMBIE_MOVE_PROB)) // chance to move
        {
            int maxAttraction = 0;
            int targetDistrictId = this->id;

            for (int neighborId : graph.getNeighbors(this->id))
            {
                int nonZombieCount = districts[neighborId]->getStateCounts()[Denizen::State::Ignorant] +
                                     districts[neighborId]->getStateCounts()[Denizen::State::Alarmed];
                int zombieCount = districts[neighborId]->getStateCounts()[Denizen::State::Zombie];

                // Define a new attraction metric
                int attraction = nonZombieCount + zombieCount; // This combines the attraction of both non-zombies and other zombies

                if (attraction > maxAttraction)
                {
                    maxAttraction = attraction;
                    targetDistrictId = neighborId;
                }
            }

            if (targetDistrictId != this->id)
            {
                denizen->setDistrictId(targetDistrictId);
            }
        }
        else
        {
            // Random movement
            moveTo(denizen, graph, districts);
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
    // Function to move a denizen based on its state
    void District::moveDenizen(Denizen *denizen, const Graph &graph, const std::vector<District *> &districts)
    {
        // Check the denizen's state and return the appropriate probability
        switch (denizen->getState())
        {
        case Denizen::State::Ignorant:
            // If the denizen is ignorant, move it to a random adjacent district
            moveTo(denizen, graph, districts);

            break;
        case Denizen::State::Alarmed:
            // If the denizen is alarmed, move it to a random adjacent district with the fewest zombies
            moveFromZombies(denizen, graph, districts);

            break;
        case Denizen::State::Zombie:
            // If the denizen is a zombie, move it to the random adjacent district with the most denizens
            moveToDenizens(denizen, graph, districts);
            break;
        }

        // return true;
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