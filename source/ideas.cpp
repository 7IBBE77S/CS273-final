
/*
    // Simulation::Simulation(int totalDays, double probX, double probY, double probZ)
    //     : districtGraph(utility::NUM_DISTRICTS), currentTime(0), totalDays(totalDays), probX(probX), probY(probY), probZ(probZ)
    // {
    //     // Initialization code
    //     initializeSimulation();
    // }
#include "district.h"

District::District(int id) : districtId(id) {}



// District::~District() {
//     for (auto* denizen : denizens) {
//         delete denizen; // Free memory for each denizen
//     }
// }

// void District::addDenizen(Denizen* denizen) {
//     denizens.push_back(denizen);
//     stateCounts[denizen->getState()]++;
// }

void District::addDenizen(std::unique_ptr<Denizen> denizen) {
    if (denizen) {
        stateCounts[denizen->getState()]++;
        denizens.push_back(std::move(denizen));
    }
}


void District::removeDenizen(int denizenId) {
    // for (auto it = denizens.begin(); it != denizens.end(); ++it) {
    //     if ((*it)->getId() == denizenId) {
    //         stateCounts[(*it)->getState()]--;
    //         delete *it; // free memory for the removed denizen
    //         denizens.erase(it);
    //         return; // successfully found and removed
    //     }
    // }

    auto it = std::find_if(denizens.begin(), denizens.end(),
                           [denizenId](const std::unique_ptr<Denizen>& denizen) { return denizen->getId() == denizenId; });
    if (it != denizens.end()) {
        stateCounts[(*it)->getState()]--;
        denizens.erase(it);
    }
}

Denizen* District::getDenizen(int denizenId) {
    // for (auto* denizen : denizens) {
    //     if (denizen->getId() == denizenId) {
    //         return denizen;
    //     }
    // }
    // return nullptr;  // return nullptr if the denizen is not found

    auto it = std::find_if(denizens.begin(), denizens.end(),
                           [denizenId](const std::unique_ptr<Denizen>& denizen) { return denizen->getId() == denizenId; });
    return it != denizens.end() ? it->get() : nullptr;
}

void District::updateDenizenState(int denizenId, Denizen::State newState) {
    Denizen* denizen = getDenizen(denizenId);
    if (denizen) {
        stateCounts[denizen->getState()]--; // decrement count of the old state
        denizen->setState(newState);        // use setter method to update state
        stateCounts[newState]++;            // increment count of the new state
    }
}

void District::update() {
    // for (Denizen* denizen : denizens) {
    //     denizen->update();
    // }
    // updateStateCounts();
     for (auto& denizen : denizens) {
        denizen->update();
    }
    updateStateCounts();
}

int District::getId() const {
    return districtId;
}

// const std::vector<Denizen*>& District::getDenizens() const {
//     return denizens;
// }


//get the denizens
const std::vector<std::unique_ptr<Denizen>>& District::getDenizens() const {
    return denizens;
}

int District::getStateCount(Denizen::State state) const {

    return stateCounts.count(state) ? stateCounts.at(state) : 0;
}

void District::updateStateCounts() {
    // reset state counts
    for (auto& pair : stateCounts) {
        pair.second = 0;
    }

    // recalculate state counts
    //  for (const Denizen* denizen : denizens) {
    //     stateCounts[denizen->getState()]++;
    // }
    for (const auto& denizen : denizens) {
        stateCounts[denizen->getState()]++;
    }
   
}

void District::initializePopulation(int ignorantCount, int alarmedCount, int zombieCount) {
    for (int i = 0; i < ignorantCount; ++i) {
        // denizens.push_back(new Denizen(Denizen::State::Ignorant, districtId));
        // stateCounts[Denizen::State::Ignorant]++;
        addDenizen(std::make_unique<Denizen>(Denizen::State::Ignorant, districtId));
    }
    // repeat for alarmedCount and zombieCount
    for (int i = 0; i < alarmedCount; ++i) {
        // denizens.push_back(new Denizen(Denizen::State::Alarmed, districtId));
        // stateCounts[Denizen::State::Alarmed]++;
        addDenizen(std::make_unique<Denizen>(Denizen::State::Alarmed, districtId));
    }

    for (int i = 0; i < zombieCount; ++i) {
        // denizens.push_back(new Denizen(Denizen::State::Zombie, districtId));
        // stateCounts[Denizen::State::Zombie]++;
        addDenizen(std::make_unique<Denizen>(Denizen::State::Zombie, districtId));
    }
}

// void District::initializePopulation(int ignorantCount, int alarmedCount, int zombieCount) {
//     for (int i = 0; i < ignorantCount; ++i) {
//         // denizens.push_back(new Denizen(Denizen::State::Ignorant, districtId));
//         // stateCounts[Denizen::State::Ignorant]++;
//         addDenizen(new Denizen(Denizen::State::Ignorant, districtId));
//     }
//     // Repeat for alarmedCount and zombieCount
//     for (int i = 0; i < alarmedCount; ++i) {
//         denizens.push_back(new Denizen(Denizen::State::Alarmed, districtId));
//         stateCounts[Denizen::State::Alarmed]++;
//     }

//     for (int i = 0; i < zombieCount; ++i) {
//         denizens.push_back(new Denizen(Denizen::State::Zombie, districtId));
//         stateCounts[Denizen::State::Zombie]++;
//     }
// }


void District::processInterDistrictMovement(const Graph& districtGraph) {
    std::vector<int> adjacentDistricts = districtGraph.getNeighbors(districtId);
    std::vector<Denizen*> denizensToMove;

    // identify denizens to move
    for (auto& denizen : denizens) {
        if (shouldDenizenMove(*denizen)) {
            denizensToMove.push_back(denizen.get());
        }
    }

    // transfer denizens to target districts
    for (Denizen* denizen : denizensToMove) {
        int targetDistrictId = selectTargetDistrict(adjacentDistricts);
        transferDenizenToDistrict(denizen, targetDistrictId);
    }

    // update the district's state after movement
    updateStateCounts();
}

bool District::shouldDenizenMove(const Denizen& denizen) {
    double moveProb = 0.0;
    switch (denizen.getState()) {
        case Denizen::State::Ignorant:
            moveProb = utility::IGNORANT_MOVE_PROB;
            break;
        case Denizen::State::Alarmed:
            moveProb = utility::ALARMED_MOVE_PROB;
            break;
        case Denizen::State::Zombie:
            moveProb = utility::ZOMBIE_MOVE_PROB;
            break;
    }
    return utility::checkProbability(moveProb);
}


int District::selectTargetDistrict(const std::vector<int>& adjacentDistricts) {
    if (adjacentDistricts.empty()) return districtId; // stay in the current district if no adjacent ones
    int index = utility::generateRandomDouble(0, adjacentDistricts.size() - 1); // randomly select an adjacent district
    return adjacentDistricts[index];
}


void District::transferDenizenToDistrict(Denizen* denizen, int targetDistrictId) {
    if (denizen == nullptr || targetDistrictId == districtId) {
        return; // no transfer needed if denizen is null or target is the current district
    }
    
    // Simulation::transferDenizen(denizen, districtId, targetDistrictId);
}


//denizens

#include "denizen.h"
#include "utility.h"
#include <chrono>




//id is not being used getid() nextid and update isnt being used
// std::atomic<int> Denizen::nextId(0);
// std::mt19937 Denizen::rng(std::chrono::system_clock::now().time_since_epoch().count());

// Denizen::Denizen(State initialState, int initialDistrict)
//     : id(nextId++), currentState(initialState), currentDistrict(initialDistrict) {}

Denizen::Denizen(State initialState, int initialDistrict)
    : currentState(initialState), currentDistrict(initialDistrict) {}

void Denizen::interactWith(Denizen& other) {
    switch (currentState) {
        case State::Ignorant:
            handleIgnorant(other);
            break;
        case State::Alarmed:
            handleAlarmed(other);
            break;
        case State::Zombie:
            handleZombie(other);
            break;
    }
}

void Denizen::update() {
    // decision to move might depend on the denizen's state and other factors
    if (shouldMove()) {
        // assuming the denizen has a reference or way to access its current district
        int targetDistrictId = currentDistrict->selectTargetDistrict();
        currentDistrict->transferDenizenToDistrict(this, targetDistrictId);
    }

}

bool Denizen::shouldMove() {
    // this could be based on the state of the denizen and conditions of the current district
    double moveProb = 0.0;
    switch (currentState) {
        case State::Ignorant:
            moveProb = utility::IGNORANT_MOVE_PROB;
            break;
        case State::Alarmed:
            moveProb = utility::ALARMED_MOVE_PROB;
            break;
        case State::Zombie:
            moveProb = utility::ZOMBIE_MOVE_PROB;
            break;
    }
    return utility::checkProbability(moveProb);
}




void Denizen::handleIgnorant(Denizen& other) {
    switch (other.getState()) {
        case State::Zombie:
            if (utility::checkProbability(utility::IGNORANT_TO_ZOMBIE_PROB)) {
                currentState = State::Zombie;  // ignorant becomes a Zombie
            } else {
                currentState = State::Alarmed; // ignorant becomes Alarmed
            }
            break;
        case State::Alarmed:
            if (utility::checkProbability(utility::IGNORANT_TO_ALARMED_PROB)) {
                currentState = State::Alarmed; // ignorant becomes Alarmed
            }
            break;
        default:
            // no action if the other is also Ignorant
            break;
    }
}

void Denizen::handleAlarmed(Denizen& other) {
    if (other.getState() == State::Zombie && utility::checkProbability(utility::ALARMED_TO_ZOMBIE_PROB)) {
        currentState = State::Zombie; // alarmed becomes a Zombie
    }
    // no action if the other is Ignorant or Alarmed
}

void Denizen::handleZombie(Denizen& other) {
    if (other.getState() != State::Zombie && utility::checkProbability(utility::ZOMBIE_BITE_SUCCESS_PROB)) {
        other.currentState = State::Zombie; // other denizen becomes a Zombie
    }
    // no action if the other is also a Zombie
}

Denizen::State Denizen::getState() const {
    return currentState;
}

void Denizen::setState(State newState) {
    currentState = newState;
}

int Denizen::getCurrentDistrict() const {
    return currentDistrict;
}

void Denizen::setCurrentDistrict(int newDistrict) {
    currentDistrict = newDistrict;
}

int Denizen::getId() const {
    return id;
}


   Handle movements (to be implemented)

     Update the state of each denizen
    for (auto& denizen : denizens) {
        denizen.updateState(denizens);
    }

    // process inter-district movements
    for (auto& denizen : denizens) {
        // determine move probability based on denizen state
        double moveProb;
        switch (denizen.getState()) {
            case State::Ignorant:
                moveProb = utility::IGNORANT_MOVE_PROB;
                break;
            case State::Alarmed:
                moveProb = utility::ALARMED_MOVE_PROB;
                break;
            case State::Zombie:
                moveProb = utility::ZOMBIE_MOVE_PROB;
                break;
        }

        // check if the denizen decides to move
        if (utility::checkProbability(moveProb)) {
            auto adjacentDistricts = graph.getNeighbors(id);
            if (!adjacentDistricts.empty()) {
                // select a random adjacent district
                int randomIndex = static_cast<int>(utility::generateRandomDouble(0, adjacentDistricts.size() - 1));
                int newDistrictId = adjacentDistricts[randomIndex];
                denizen.moveTo(newDistrictId);
            }
        }
        
    }

       void Simulation::outputToCSV(const std::string &filename)
    {
        std::cout << "Attempting to create output file: " << filename << std::endl;
        std::ofstream file(filename);

        if (!file.is_open())
        {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return;
        }

        std::cout << "Writing headers to file." << std::endl;
        std::cout << "Finished writing to file." << std::endl;

        file.close();
        std::cout << "File closed." << std::endl;
    }

    void Simulation::initializeSimulation()
    {
        //  ensure all nodes (districts) are added to the graph
        for (int i = 0; i < utility::NUM_DISTRICTS; ++i)
        {
            districtGraph.addNode(i); // Assuming addNode exists
        }
        //layout of the simulation area using the Graph
        // debug couts
        std::cout << "Adding edge between DOWNTOWN and MEDICAL_HILL" << std::endl;
        districtGraph.addEdge(utility::DOWNTOWN, utility::MEDICAL_HILL);
        std::cout << "Adding edge between  MEDICAL HILL AND THE BURBS" << std::endl;
        districtGraph.addEdge(utility::MEDICAL_HILL, utility::THE_BURBS);
        std::cout << "Adding edge between THE BURBS AND UNIVERSITY" << std::endl;
        districtGraph.addEdge(utility::THE_BURBS, utility::UNIVERSITY);

        // create districts and distribute initial denizen populations
        for (int i = 0; i < utility::NUM_DISTRICTS; ++i)
        {
            District newDistrict(i);
            if (i == utility::MEDICAL_HILL)
            {
                newDistrict.initializePopulation(100, 10, 1);
            }
            else
            {
                newDistrict.initializePopulation(100, 10, 0);
            }
            districts.emplace(i, newDistrict);
            //   districts.emplace(std::make_ (i, std::move(newDistrict)));
        }

       
        // districts[UNIVERSITY].setAlarmedPerson(); // assuming such a method exists


        // eventQueue.push(Event(Event::EventType::OutbreakStart, MEDICAL_HILL, 0, 0));

        // eventQueue.push(Event(Event::EventType::StateChange, utility::MEDICAL_HILL, utility::PATIENT_ZERO_ID, 0));
    }

    void Simulation::processEvent(const Event &event)
    {
        std::cout << "Processing event type: " << static_cast<int>(event.type) << std::endl;
        switch (event.type)
        {
        case Event::EventType::Move:
            std::cout << "Handling Move Event" << std::endl;
            moveDenizens();
            break;
        case Event::EventType::Interaction:
            std::cout << "Handling Interaction Event" << std::endl;
            handleInteraction(event);
            break;
        case Event::EventType::StateChange:
            std::cout << "Handling State Change Event" << std::endl;
            handleStateChange(event);
            break;
        }
    }

    void Simulation::handleInteraction(const Event &event)
    {
        Denizen *actor = districts[event.districtId].getDenizen(event.actorId);
        Denizen *target = districts[event.districtId].getDenizen(event.targetId);

        if (actor && target)
        {
            if (actor->getState() == Denizen::State::Zombie && target->getState() != Denizen::State::Zombie)
            {
                // zombie biting another denizen
                if (utility::checkProbability(utility::ZOMBIE_BITE_SUCCESS_PROB))
                {
                    target->setState(Denizen::State::Zombie); // direct state change
                }
            }
            else if (actor->getState() == Denizen::State::Alarmed && target->getState() == Denizen::State::Ignorant)
            {
                // alarmed person warning an Ignorant person
                if (utility::checkProbability(utility::IGNORANT_TO_ALARMED_PROB))
                {
                    target->setState(Denizen::State::Alarmed); // direct state change
                }
            }
        }
    }

    void Simulation::handleStateChange(const Event &event)
    {
        Denizen *denizen = districts[event.districtId].getDenizen(event.targetId);

        if (denizen)
        {
            // change the state of the denizen based on the event
            denizen->setState(event.newState);
        }
    }

    void Simulation::updateDistricts()
    {
        for (auto &[id, district] : districts)
        {
            district.update(); // this might involve denizens interacting within the district
        }
    }

    void Simulation::moveDenizens()
    {
        std::cout << "Moving denizens between districts." << std::endl;
        std::vector<std::tuple<int, int, int>> movements; // (fromDistrict, toDistrict, denizenId)

        for (auto &[districtId, district] : districts)
        {
            for (const auto &denizen : district.getDenizens())
            {
                // determine move probability based on denizen state
                double moveProb;
                switch (denizen->getState())
                {
                case Denizen::State::Ignorant:
                    moveProb = utility::IGNORANT_MOVE_PROB;
                    break;
                case Denizen::State::Alarmed:
                    moveProb = utility::ALARMED_MOVE_PROB;
                    break;
                case Denizen::State::Zombie:
                    moveProb = utility::ZOMBIE_MOVE_PROB;
                    break;
                }

                // check if the denizen decides to move
                if (utility::checkProbability(moveProb))
                {
                    auto adjacentDistricts = districtGraph.getNeighbors(districtId);
                    if (!adjacentDistricts.empty())
                    {
                        // select a random adjacent district
                        int randomIndex = static_cast<int>(utility::generateRandomDouble(0, adjacentDistricts.size() - 1));
                        int newDistrictId = adjacentDistricts[randomIndex];
                        movements.emplace_back(districtId, newDistrictId, denizen->getId());
                    }
                }
            }
        }
        // apply the movements
        for (const auto &[fromDistrict, toDistrict, denizenId] : movements)
        {
            districts[fromDistrict].removeDenizen(denizenId);
            districts[toDistrict].addDenizen(new Denizen(*districts[fromDistrict].getDenizen(denizenId))); // assuming copy constructor
        }
    }

*/