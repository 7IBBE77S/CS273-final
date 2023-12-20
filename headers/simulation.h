#ifndef SIMULATION_H
#define SIMULATION_H

#include "district.h"
#include "denizen.h"
#include "graph.h"
#include <vector>
#include <memory>
#include <string>
#include <fstream>
#include <queue>



namespace simulation
{
     // Define an enum for different event types
    enum EventType {
        Move,
        Interaction,
        StateChange
    };

    // Structure to represent an event
    struct Event {
        EventType type;
        // int denizenId; // ID of the denizen involved in the event
        // int targetDistrictId; // Relevant for Move events

        // Constructor for creating an event
        // Event(EventType type, int denizenId, int targetDistrictId = -1)
        //     : type(type), denizenId(denizenId), targetDistrictId(targetDistrictId) {}
    };

    class Simulation
    {
    public:
        // initializes the simulation with the total number of days and the graph.
        Simulation(int numDays, const Graph &graph);

      

        // ~Simulation();

        // initialize the simulation: Set up districts, denizens, and initial conditions.
        void initialize();

        // run the simulation for the specified number of days.
        void run();

        // update the simulation for one time step (an hour).
        // void update();

        // add a district to the simulation.
        void addDistrict(District *district);

        void handleDenizenMovements();
        void distributeInitialPopulation();
        void displaySimulationState();
        // void configureDistricts(); // Set up initial conditions for each district... maybe not needed

        // const std::vector<District>& getDistricts() const { return districts; } maybe needed?

        // get the state of the simulation, districts, and denizens.
        // these can include methods to retrieve data for reporting or visualization.
        void outputToCSV(const std::string &filename)
        {
            std::ofstream file(filename, std::ios::app);

            // Header for the CSV file
            file << "\nDay " << currentDay << " Summary:\n";

            file << "Hour, District ID, Ignorant, Alarmed, Zombie\n";

            for (const auto &district : districts)
            {
                auto stateCounts = district->getStateCounts();
                // currentTimeStep

                file << std::setw(10) << currentTimeStep << ", "
                     << std::setw(12) << district->getId() << ", "
                     << std::setw(8) << stateCounts[Denizen::State::Ignorant] << ", "
                     << std::setw(7) << stateCounts[Denizen::State::Alarmed] << ", "
                     << std::setw(6) << stateCounts[Denizen::State::Zombie] << "\n";
            }

            // Add a line break for readability between time steps
            file << "\n";
        }

    private:
        int numDays;                       // total number of days for the simulation
        Graph graph;                       // graph representing the layout of districts
        std::vector<District *> districts; // vector containing all districts in the simulation
        std::queue<Event> eventQueue;
        int currentDay;                    // current day in the simulation
        int currentTimeStep;               // current time step (hour) within the current day
        int nextDenizenId;                 // Unique identifier for each denizen

        // initialize districts with denizens and their states.
        void initializeDistricts();

        // Update each district's state and denizen interactions for the current time step.
        void updateDistricts();

// Private methods for handling different types of events
        void handleMoveEvent(const Event& event);
        void handleInteractionEvent(const Event& event);
        void handleStateChangeEvent(const Event& event);

        // Method to schedule an event
        void scheduleEvent(const Event& event);

       
    };

}

#endif
