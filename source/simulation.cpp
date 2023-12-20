#include "simulation.h"
#include "denizen.h"
#include "zombie.h"
#include "alarmed.h"
#include "ignorant.h"
#include "colors.h"
#include "utility.h"
#include <fstream>
#include <iostream>
#include <vector>
#include "graph.h"
#include <thread>

namespace simulation
{
    // this constructor is not being used and is pseudo right now

    // Simulation::Simulation(int totalDays, double probX, double probY, double probZ)
    //     : districtGraph(utility::NUM_DISTRICTS), currentTime(0), totalDays(totalDays), probX(probX), probY(probY), probZ(probZ)
    // {
    //     // Initialization code
    //     initializeSimulation();
    // }
    Simulation::Simulation(int numDays, const Graph &graph) : numDays(numDays), graph(graph), currentDay(0), currentTimeStep(0)
    {
        // initializeSimulation();
    }

    // Simulation::~Simulation() {
    //     for (auto &district : districts) {
    //         for (Denizen* denizen : district->getDenizens()) {
    //             delete denizen;  // Free the memory
    //         }
    //     }
    // }

    void Simulation::addDistrict(District *district)
    {
        //  revise the addDenizen method in the District class to update the districtId of denizens when they are added to a new district, or ensure this update is handled in handleDenizenMovements before the denizen is added to the new district.
        districts.push_back(district);
    }

    void Simulation::updateDistricts()
    {
        // iterate through each district and update it
        for (auto &district : districts)
        {
            district->update(graph);
        }

        // after updating all districts, handle the movements of denizens
        handleDenizenMovements();
    }

    void clearTerminal()
    {
#ifdef __APPLE__
        std::system("clear");

#else
        std::system("cls");
#endif
    }
    std::string stateToString(Denizen::State state)
    {
        switch (state)
        {
        case Denizen::State::Ignorant:
            return "Ignorant";
        case Denizen::State::Alarmed:
            return "Alarmed";
        case Denizen::State::Zombie:
            return "Zombie";
        default:
            return "Unknown?!";
        }
    }
    void Simulation::displaySimulationState()
    {
        clearTerminal(); // Clear the terminal for a clean display

        const char *zombieT = R"(
                     ..... .., . ., .   .,....,,.  
                ... . . .. .....  ...,,,,,,.,,,..... . .    
         . . ... . .....   .....,,,,,,,,,,,,,,,,,,.....
             .  ..........,,,,,,,,,,,,,,.,,,,,,,,,,....,, ..               
      ... ..  ....   ,.,,.,.,,.,,.,,,,,,,.,,,.,,,,,,,...., ..... ...      
     .           .  ...,.,,,,,.....,.....,,...,....,,,,,,..  .. ........   
          ..     .,.,...,,.,......,..................,,..,. ... .  ...,.            
      .  .. ................. .,..........................,.....  .,. ... . .   
     .  . ..  . ..............  ....................,..........   ......... .   
     ... . .. ... ......  .... ...................................  . .   ..    
         . . .   ....  ...... ...................... ........  . .....     .    
     . ..  ....  ................... .................... ..   ,  . ....   ..   
      .      ...  ... ........................................      . . ..  .   
      . .        ..  ........  ............ ...................   .     .       
     ..  .  ...    ......     .   ........ .. ....  .     .....  .. . .         
           .   .  . .... . .         . ..,...                .      .    )";
        std::cout << colors.Zombie << zombieT << colors.Clean << std::endl;
        std::cout << colors.Zombie << "  ..  .  ..    .. . .";
        std::cout << colors.Red << "       /0\\" << colors.Clean;
        std::cout << colors.Zombie << "     . .  .,...  .";
        std::cout << colors.Red << "     /0\\" << colors.Clean;
        std::cout << colors.Zombie << "       ..     ." << std::endl;
        std::cout << colors.Zombie << "   .   .     .    . . .";
        std::cout << colors.dim << "     \\0/" << colors.Clean;
        std::cout << colors.Zombie << "      .  ,,   . .";
        std::cout << colors.dim << "      \\0/" << colors.Clean;
        std::cout << colors.Zombie << "      ..";
        const char *zombieB = R"(
     .. .       .. ... ..        ......,     , ...          ...       ..    .   
      .  .     .   ........   ....... .       .......       ...                 
       ..  ..     ..... .......  .  ...   .  . ..       .,.... .                
     .      .    ......, .. ..   .....    ..   ...       . , . ..               
        .  .       ....,         ... ..  ...  ... .         . .        .        
              .        .         .................         .                    
            .                    ..................                             
         .                      .. .. ,. .. ....   .                            
                            ..    ............. .. .  .                         
          .                 .      . . .......  .     .                         
                            ... .                  . .                          
      .                     . .. .                .                             
          .                 ....  ...          ,... . .                         
     .           .           ...... ........   .     .                          
                              ........... . .     ..                            
                                 .... . ..                                      
           .                    . ................                              
                                     ........ .                                 
      .                               . ....                                    
             .                        .  ..                         
                            
  )";
        std::cout << colors.Zombie << zombieB << colors.Clean << std::endl;

        std::cout << "Day " << currentDay << ", Hour " << currentTimeStep << ":" << std::endl;
        int totalSurvivors = 0; // Variable to keep track of total survivors

        for (const auto &district : districts)
        {
            auto stateCounts = district->getStateCounts();
            // now a check to see if state counts are empty and if so we will display dead by the name of the state change
            int ignorantCount = stateCounts[Ignorant::State::Ignorant];
            int alarmedCount = stateCounts[Alarmed::State::Alarmed];
            int zombieCount = stateCounts[Zombie::State::Zombie];

            totalSurvivors += ignorantCount + alarmedCount;
            // Display district states
            std::cout << district->getName() << ": ";
            std::cout << "[Ignorant: " << colors.White << ignorantCount << " " << colors.GreyBG << std::string(ignorantCount / 5, '_') << colors.Clean << "] ";
            std::cout << "[Alarmed: " << colors.gold << alarmedCount << " " << colors.BGorange << std::string(alarmedCount / 5, '_') << colors.Clean << "] ";
            std::cout << "[Zombie: " << colors.Red << zombieCount << " " << colors.BGred << std::string(zombieCount / 5, '_') << colors.Clean << "]" << std::endl;
        }
        std::cout << "\n[Survivors: " << totalSurvivors << " ]" << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(0)); // Delay for visual effect change number as needed
    }
    // Function to handle the movement of denizens between districts
    void Simulation::handleDenizenMovements()
    {
        // Create a map to track the movements of denizens
        std::unordered_map<int, std::vector<std::pair<Denizen *, int>>> movementsMap;

        // Identify denizens to move and track them
        for (auto &district : districts)
        {
            auto &denizens = district->getDenizens();
            for (auto &denizen : denizens)
            {
                // If the denizen should move
                if (district->shouldMove(*denizen))
                {

                    //     // Determine the new district ID for the denizen
                    // int newDistrictId = district->determineNewDistrictId(*denizen, graph);
                    //     // Add the denizen and the new district ID to the movements map
                    district->moveDenizen(denizen, graph, districts);
                    movementsMap[district->getId()].emplace_back(denizen, denizen->getDistrictId());
                }
            }
        }

        // Move denizens to their new districts
        for (const auto &move : movementsMap)
        {
            int originalDistrictId = move.first;
            for (const auto &denizenPair : move.second)
            {
                Denizen *denizen = denizenPair.first;
                int newDistrictId = denizenPair.second;

                // Safely transfer denizen to the new district
                districts[originalDistrictId]->removeDenizen(denizen->getId());
                denizen->setDistrictId(newDistrictId);
                districts[newDistrictId]->addDenizen(denizen);
            }
        }
    }

    // Function to distribute the initial population of denizens across the districts
    void Simulation::distributeInitialPopulation()
    {
        // Calculate the population for each district, assuming an even distribution
        int populationPerDistrict = utility::NUM_DENIZENS / districts.size();

        for (auto &district : districts)
        {
            for (int i = 0; i < populationPerDistrict; ++i)
            {
                Denizen *newDenizen = nullptr;

                // Create a new denizen with the appropriate state based on the district ID and denizen ID
                if (district->getId() == utility::MEDICAL_HILL && i == utility::PATIENT_ZERO_ID)
                {
                    newDenizen = new Zombie(nextDenizenId++, district->getId(), Denizen::State::Zombie);
                }
                else if (district->getId() == utility::UNIVERSITY && i == utility::ALARMED_DENIZEN_ID)
                {
                    newDenizen = new Alarmed(nextDenizenId++, district->getId(), Denizen::State::Alarmed);
                }
                else
                {
                    newDenizen = new Ignorant(nextDenizenId++, district->getId(), Denizen::State::Ignorant);
                }

                // Add the new denizen to the district
                district->addDenizen(newDenizen);
            }
        }
    }

    // Function to initialize the simulation
    void Simulation::initialize()
    {
        // Add districts to the simulation
        for (int i = 0; i < utility::NUM_DISTRICTS; ++i)
        {
            // Create a new district with the appropriate name based on the district ID
            switch (i)
            {
            case utility::MEDICAL_HILL:
            {
                std::string districtName = "Medical Hill";
                addDistrict(new District(i, districtName));
                break;
            }
            case utility::THE_BURBS:
            {
                std::string districtName = "The Burbs";
                addDistrict(new District(i, districtName));
                break;
            }
            case utility::UNIVERSITY:
            {
                std::string districtName = "The University";
                addDistrict(new District(i, districtName));
                break;
            }
            case utility::DOWNTOWN:
            {
                std::string districtName = "Downtown";
                addDistrict(new District(i, districtName));
                break;
            }
            default:
            {
                std::string districtName = "District test " + std::to_string(i + 1);
                addDistrict(new District(i, districtName));
                break;
            }
            }
        }
    }

    // Function to run the simulation
    void Simulation::run()
    {
        // Create a CSV file to store the simulation output
        std::string filename = "simulation_output.csv";
        std::ofstream file(filename);
        file.close();

        // Distribute the initial population of denizens
        distributeInitialPopulation();

        // Output the initial state of the simulation to the CSV file
        outputToCSV(filename);

        // Run the simulation for the specified number of days
        for (currentDay = 1; currentDay <= numDays; ++currentDay)
        {
            // Each day consists of 24 time steps
            for (currentTimeStep = 1; currentTimeStep <= 24; ++currentTimeStep)
            {
                // Update the state of the districts
                updateDistricts();
                // Output the current state of the simulation to the CSV file
                outputToCSV(filename);
                // Display the current state of the simulation
                displaySimulationState();
            }
        }
    }

}