// #include "denizen.h"
// #include "utility.h"
// #include <random>

// namespace simulation {


// tell me literally everything about this function and what it does
//  
// void Denizen::updateState(const std::vector<Denizen>& otherDenizens) {
//     for (const auto& other : otherDenizens) {
//         if (id != other.getId()) { // ensure not to interact with itself
//             // ignorant denizen encounters 
//             if (currentState == State::Ignorant) {
//                 if (other.getState() == State::Zombie && utility::checkProbability(utility::IGNORANT_TO_ZOMBIE_PROB)) {
//                     currentState = State::Zombie;
//                     return; // change to Zombie ends further interactions
//                 } else if (other.getState() == State::Alarmed && utility::checkProbability(utility::IGNORANT_TO_ALARMED_PROB)) {
//                     currentState = State::Alarmed;
//                     return; // change to Alarmed ends further interactions
//                 }
//             }
//             // alarmed denizen encounters
//             else if (currentState == State::Alarmed) {
//                 if (other.getState() == State::Zombie && utility::checkProbability(utility::ALARMED_TO_ZOMBIE_PROB)) {
//                     currentState = State::Zombie;
//                     return; // change to Zombie ends further interactions
//                 }
//             }
//         }
//     }
// }
// void Denizen::moveTo(int newDistrictId, const Graph &graph) {
//     if (graph.areAdjacent(districtId, newDistrictId)) {
//         setDistrictId(newDistrictId); // this line will update the internal districtId of the Denizen
//     }
// }



// } 