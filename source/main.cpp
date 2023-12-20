/************************************************************
 *  Author:         Nicholas Tibbetts
 *  Date:           11/16/2023 T23:55:46
 *  Course Code:    CS-273
 *  License:        Copyright 2023 Nic Tibbetts
 *  Resources used: _
 *  Description:    28 Days Later Zombie Apocalypse!
 *  Version:        5.0
 * 
 *  Special Thanks:  Christopher Strand
 ***********************************************************/

#include "simulation.h"
#include "graph.h"
#include "district.h"
#include "denizen.h"
#include "utility.h"
#include <iostream>
#include <string>

using namespace simulation;

int main()
{
 
  // move a lot this crap into simulation file

  //set up the graph representing district connections
  Graph districtGraph(false); // Assuming a non-directed graph

  for (int i = 0; i < utility::NUM_DISTRICTS; i++)
  {
    districtGraph.addNode(i);
  }

  for (int i = 0; i < utility::NUM_DISTRICTS; i++)
  {
    for (int j = 0; j < utility::NUM_DISTRICTS; j++)
    {
      if (i != j)
      {
        districtGraph.addEdge(i, j);
      }
    }
  }

  // now combine both the addNode and addEdge for loops into one for loop

  // create the Simulation instance
  Simulation zombieSimulation(utility::NUM_DAYS, districtGraph);

  zombieSimulation.initialize();

  try
  {
    zombieSimulation.run();
    // zombieSimulation.outputToCSV("simulation_output.csv");
  }
  catch (const std::exception &e)
  {
    std::cerr << "An error occurred: " << e.what() << std::endl;
    return 1;
  }

  std::cout << "Simulation completed successfully." << std::endl;
  return 0;
}