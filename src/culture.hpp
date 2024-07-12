#pragma once

#include "grid.hpp"
#include "output.hpp"
#include "pcg_random.hpp"
#include "position.hpp"

#include <cstdint>
#include <random>
#include <tuple>
#include <unordered_map>
#include <vector>

struct CellData {
    bool is_active;
    bool is_stem;
    int parent_index;
};

class Culture {
public:
    Culture(float cell_radius, Output* output = nullptr, uint64_t seed = 110293658491283598)
        : cell_radius_ { cell_radius }
        , grid_ { 2.0f * cell_radius }
        , rng_ { seed }
        , output_ { output }
    {
    }

    enum class BoundaryConditions { NONE };

    float adjacency_threshold = 4.0f;
    float prob_stem = 0.0f;
    float prob_diff = 0.0f;
    float prob_swap = 0.5f;
    int cell_max_repro_attempts = 1000;

    int addCell(const Position position, int parent_index, bool is_stem, bool is_active = true);
    int simulate(int time_steps);

private:
    Position generateNewPosition(const Position& parent);
    std::tuple<bool, bool> determineStemness(const bool parent_was_stem);
    bool overlapsNeighbors(const Position& candidate_position, const Position& parent_position);
    void reproduce(int parent_index);
    float adjustCoordinate(float c) const;
    void addOutput(Position pos, bool is_stem, bool is_active);

    std::vector<Position> cell_positions_;
    std::vector<CellData> cell_data_;

    float cell_radius_;
    Grid grid_;

    BoundaryConditions boundary_conditions = BoundaryConditions::NONE;

    // Culture RNG
    pcg32 rng_;
    std::uniform_real_distribution<float> cos_theta_dist_ { -1.0f, 1.0f };
    std::uniform_real_distribution<float> phi_dist_ { 0.0f, 2.0f * M_PI };
    std::uniform_real_distribution<float> uniform_dist_ { 0.0f, 1.0f };

    // Timekeeping
    int tic_ = 0;
    int repro_ = 0;
    int attempt_ = 0;

    // Output
    Output* output_;
};
