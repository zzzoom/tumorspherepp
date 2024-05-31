#pragma once

#include "neighborhood.hpp"
#include "position.hpp"

#include <cstdint>
#include <tuple>
#include <unordered_map>

using GridCoord = int32_t;
using CellId = std::tuple<GridCoord, GridCoord, GridCoord>;

namespace std {
template <>
struct hash<CellId> {
    size_t operator()(const CellId& cell) const
    {
        size_t h1 = std::hash<int32_t>()(std::get<0>(cell));
        size_t h2 = std::hash<int32_t>()(std::get<1>(cell));
        size_t h3 = std::hash<int32_t>()(std::get<2>(cell));
        return h1 ^ (h2 << 3) ^ (h3 << 6); // Combine hashes
    }
};
}

class Grid {
public:
    Grid(float cell_size)
        : cell_size_(cell_size)
        , neighborhoods_(1000003)
    {
    }
    void addPosition(const Position& pos);
    float minSquaredDistance(const Position& pos, const Position& parent);

private:
    float cell_size_;
    std::unordered_map<CellId, Neighborhood> neighborhoods_;

    GridCoord quantizeCoordinate(float coord) const;
};