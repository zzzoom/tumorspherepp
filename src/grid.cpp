#include "grid.hpp"

#include <cmath>

void Grid::addPosition(const Position& pos)
{
    auto bucket_x = quantizeCoordinate(pos.x);
    auto bucket_y = quantizeCoordinate(pos.y);
    auto bucket_z = quantizeCoordinate(pos.z);

    for (GridCoord z = bucket_z - 1; z <= bucket_z + 1; ++z) {
        for (GridCoord y = bucket_y - 1; y <= bucket_y + 1; ++y) {
            for (GridCoord x = bucket_x - 1; x <= bucket_x + 1; ++x) {
                CellId cell = std::make_tuple(bucket_x, bucket_y, bucket_z);
                neighborhoods_.try_emplace(cell);
                neighborhoods_[cell].addPosition(pos.x, pos.y, pos.z);
            }
        }
    }
}

float Grid::minSquaredDistance(const Position& pos, const Position& parent)
{
    auto bucket_x = quantizeCoordinate(pos.x);
    auto bucket_y = quantizeCoordinate(pos.y);
    auto bucket_z = quantizeCoordinate(pos.z);
    CellId cell = std::make_tuple(bucket_x, bucket_y, bucket_z);
    auto neighborhood = neighborhoods_[cell];
    bool found_parent = neighborhood.swapToFront(parent.x, parent.y, parent.z);
    if (!found_parent) {
        // TODO: log/error
    }
    return neighborhood.minSquaredDistance(true, pos.x, pos.y, pos.z);
}

GridCoord Grid::quantizeCoordinate(float coord) const
{
    return static_cast<GridCoord>(floorf(coord / cell_size_));
}