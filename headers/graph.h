#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <vector>
#include <stdexcept>
#include "utility.h"

class Graph
{
public:
    Graph(bool isDirected) : isDirected(isDirected) {}

    void addNode(int nodeId)
    {
        if (!hasNode(nodeId))
        {
            adjacencyList[nodeId] = std::vector<int>();
        }
    }

    void addEdge(int from, int to)
    {
        if (!hasNode(from) || !hasNode(to))
        {
            throw std::invalid_argument("One or more nodes do not exist.");
        }
        adjacencyList[from].push_back(to);
        if (!isDirected)
        {
            adjacencyList[to].push_back(from);
        }
    }

    bool areAdjacent(int from, int to) const
    {
        auto it = adjacencyList.find(from);
        if (it != adjacencyList.end())
        {
            return std::find(it->second.begin(), it->second.end(), to) != it->second.end();
        }
        return false;
    }

    std::vector<int> getNeighbors(int nodeId) const
    {
        auto it = adjacencyList.find(nodeId);
        return it != adjacencyList.end() ? it->second : std::vector<int>();
    }

    bool hasNode(int nodeId) const
    {
        return adjacencyList.find(nodeId) != adjacencyList.end();
    }

   

private:
    std::unordered_map<int, std::vector<int>> adjacencyList; // first int is the district id, the vector holds ints of adjacent districts
    bool isDirected;
};

#endif
/*
add a method to ensure that the graph remains connected. This could help prevent isolated districts.
*/