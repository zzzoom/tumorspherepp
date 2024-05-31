#pragma once

#include <vector>

class Neighborhood {
public:
    Neighborhood() { }

    void addPosition(float x, float y, float z);
    bool swapToFront(float x, float y, float z);
    float minSquaredDistance(bool ignore_first, float x, float y, float z) const;

private:
    std::vector<float> xcoords_;
    std::vector<float> ycoords_;
    std::vector<float> zcoords_;
};
