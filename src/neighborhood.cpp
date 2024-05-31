#include "neighborhood.hpp"

#include <limits>
#include <utility>

void Neighborhood::addPosition(float x, float y, float z)
{
    xcoords_.push_back(x);
    ycoords_.push_back(y);
    zcoords_.push_back(z);
}

bool Neighborhood::swapToFront(float x, float y, float z)
{
    auto len = xcoords_.size();
    for (decltype(len) i = 0; i < len; ++i) {
        if ((x == xcoords_[i]) && (y == ycoords_[i]) && (z == zcoords_[i])) {
            if (i != 0) {
                std::swap(xcoords_[i], xcoords_[0]);
                std::swap(ycoords_[i], ycoords_[0]);
                std::swap(zcoords_[i], zcoords_[0]);
            }
            return true;
        }
    }
    return false;
}

float Neighborhood::minSquaredDistance(bool ignore_first, float x, float y, float z) const
{
    float result = std::numeric_limits<float>::max();
    auto len = xcoords_.size();
    decltype(len) start = ignore_first ? 1 : 0;
    for (auto i = start; i < len; ++i) {
        float difx = x - xcoords_[i];
        float dify = y - ycoords_[i];
        float difz = z - zcoords_[i];
        result = std::min(result, difx * difx + dify * dify + difz * difz);
    }
    return result;
}
