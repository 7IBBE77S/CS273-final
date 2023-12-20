
// struct Edge
// {
//     int target;
//     double weight;
//     Edge(int target, double weight) : target(target), weight(weight) {}
// };

// Graph(bool isDirected) : isDirected(isDirected) {}

// void addNode(int nodeId)
// {
//     if (!hasNode(nodeId))
//     {
//         adjacencyList[nodeId] = std::vector<Edge>();
//     }
// }

// void addEdge(int from, int to, double weight)
// {
//     if (!hasNode(from) || !hasNode(to))
//     {
//         throw std::invalid_argument("One or more nodes do not exist.");
//     }
//     adjacencyList[from].emplace_back(to, weight);
//     if (!isDirected)
//     {
//         adjacencyList[to].emplace_back(from, weight);
//     }
// }

// bool areAdjacent(int from, int to) const
// {
//     auto it = adjacencyList.find(from);
//     if (it != adjacencyList.end())
//     {
//         for (const Edge &edge : it->second)
//         {
//             if (edge.target == to)
//             {
//                 return true;
//             }
//         }
//     }
//     return false;
// }

// std::vector<Edge> getNeighbors(int nodeId) const
// {
//     auto it = adjacencyList.find(nodeId);
//     return it != adjacencyList.end() ? it->second : std::vector<Edge>();
// }

// bool hasNode(int nodeId) const
// {
//     return adjacencyList.find(nodeId) != adjacencyList.end();
// }

// private:
// std::unordered_map<int, std::vector<Edge>> adjacencyList;
// bool isDirected;

// #ifndef DISTRICT_H
// #define DISTRICT_H

// #include "denizen.h"
// #include "graph.h"
// #include <vector>
// #include <unordered_map>

// class District
// {
// public:
//     District(int id = -1);
//     ~District() = default; // destructor is not needed with smart pointers...

//     void addDenizen(std::unique_ptr<Denizen> denizen);
//     void removeDenizen(int denizenId);
//     Denizen *getDenizen(int denizenId);
//     void updateDenizenState(int denizenId, Denizen::State newState);
//     void update(); // Update state of district and denizens within it

//     int getId() const;
//     const std::vector<std::unique_ptr<Denizen>> &getDenizens() const;
//     int getStateCount(Denizen::State state) const;

//     void initializePopulation(int ignorantCount, int alarmedCount, int zombieCount); // Placeholder for population initialization
//     void processInterDistrictMovement(const Graph &districtGraph);
//     bool shouldDenizenMove(const Denizen &denizen);
//     int selectTargetDistrict(const std::vector<int> &adjacentDistricts);
//     void transferDenizenToDistrict(Denizen *denizen, int targetDistrictId);

// private:
//     int districtId;
//     // std::vector<std::unique_ptr<Denizen>> denizens;
//     // std::vector<Denizen> denizens;
//     std::vector<Denizen *> denizens;
//     std::unordered_map<Denizen::State, int> stateCounts; // Track the number of denizens in each state
//                                                          // std::vector<District*> adjacentDistricts
//     void updateStateCounts();
// };

// #endif // DISTRICT_H
