#include "visualization.h"
#include <fstream>

//old idea
namespace visualization {

    void outputToCSV(const std::string& filename, const std::vector<std::vector<int>>& data) {
        std::ofstream file(filename);
        for (const auto& row : data) {
            for (size_t i = 0; i < row.size(); ++i) {
                file << row[i];
                if (i < row.size() - 1) file << ",";
            }
            file << "\n";
        }
        file.close();
    }


}
