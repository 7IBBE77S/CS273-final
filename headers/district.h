#ifndef DISTRICT_H
#define DISTRICT_H

#include "denizen.h"
#include "ignorant.h"
#include "alarmed.h"
#include "zombie.h"
#include "graph.h"
#include <vector>
#include <string>
#include <unordered_map>

namespace simulation
{

  class District
  {
  public:
    District(int id, const std::string &name);

    ~District();

    void addDenizen(Denizen *denizen);
    void removeDenizen(int denizenId);
    void update(const Graph &graph);

    int getId() const { return id; }
    const std::string &getName() const { return name; }
    std::vector<Denizen *> &getDenizens() { return denizens; }
    std::unordered_map<Denizen::State, int> getStateCounts() const { return stateCounts; }
    bool canMoveToNewDistrict(const Denizen &denizen, const Graph &graph);
    Denizen *updateState(Denizen *&denizen);

    bool shouldMove(const Denizen &denizen);
    int determineNewDistrictId(const Denizen &denizen, const Graph &graph);

    template <typename OldState, typename NewState>
    NewState* createNewDenizen(OldState *oldDenizen);

    // void processInteraction(Denizen& denizen, const Denizen& other);

  private:
    int id;                                              // unique identifier for the district
    std::string name;                                    // name of the district
    std::vector<Denizen *> denizens;                     // list of denizens in the district
    std::unordered_map<Denizen::State, int> stateCounts; // track the number of denizens in each state
  };

}

#endif
