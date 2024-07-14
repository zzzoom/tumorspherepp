#include "culture.hpp"

#include "pcg_random.hpp"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <random>
#include <ranges>
#include <tuple>
#include <vector>

float Culture::adjustCoordinate(float c) const
{
    float result = c;
    /* TODO: torus */
    return result;
}

Position Culture::generateNewPosition(const Position& parent)
{
    // Choose a random mean between 1 and 6
    float cos_theta = cos_theta_dist_(rng_);
    float theta = acosf(cos_theta);
    float sin_theta = sin(theta);

    float phi = phi_dist_(rng_);
    float sin_phi = sin(phi);
    float cos_phi = cos(phi);

    return Position {
        adjustCoordinate(parent.x + 2 * cell_radius_ * sin_theta * cos_phi),
        adjustCoordinate(parent.y + 2 * cell_radius_ * sin_theta * sin_phi),
        adjustCoordinate(parent.z + 2 * cell_radius_ * cos_theta)
    };
}

std::tuple<bool, bool> Culture::determineStemness(const bool parent_was_stem)
{
    bool parent_is_stem = parent_was_stem;
    bool child_is_stem = false;
    if (!parent_was_stem) {
        parent_is_stem = false;
        child_is_stem = false;
    } else {
        float roll1 = uniform_dist_(rng_);
        if (roll1 <= prob_stem) {
            parent_is_stem = true;
            child_is_stem = true;
        } else if (roll1 <= prob_stem + prob_diff) {
            parent_is_stem = false;
            child_is_stem = false;
        } else {
            float roll2 = uniform_dist_(rng_);
            if (roll2 <= prob_swap) {
                parent_is_stem = false;
                child_is_stem = true;
            } else {
                parent_is_stem = true;
                child_is_stem = false;
            }
        }
    }
    return std::tuple { parent_is_stem, child_is_stem };
}

bool Culture::overlapsNeighbors(const Position& candidate_position, const Position& parent_position)
{
    auto distance_squared = grid_.minSquaredDistance(candidate_position, parent_position);
    return distance_squared <= (4.0f * cell_radius_ * cell_radius_);
}

int Culture::addCell(const Position position, int parent_index, bool is_stem, bool is_active)
{
    int cell_index = cell_data_.size();

    cell_positions_.push_back(position);
    grid_.addPosition(position);
    CellData cell_data { .is_active = is_active, .is_stem = is_stem, .parent_index = parent_index };
    cell_data_.push_back(cell_data);

    if (output_) {
        output_->addCell(cell_index, parent_index, position);
        output_->updateStatus(cell_index, tic_, repro_, attempt_, is_stem, is_active);
    }
    return cell_index;
}

void Culture::reproduce(int parent_index)
{
    const Position& parent_position = cell_positions_[parent_index];
    CellData& parent_data = cell_data_[parent_index];

    if (!parent_data.is_active) {
        return;
    }

    bool reproduced = false;
    for (int attempt_ = 0; attempt_ < cell_max_repro_attempts; ++attempt_) {
        const Position child_position = generateNewPosition(parent_position);
        // clampPosition(child_position);

        if (!overlapsNeighbors(child_position, parent_position)) {
            const bool parent_was_stem = parent_data.is_stem;
            const auto [parent_is_stem, child_is_stem] = determineStemness(parent_was_stem);

            // TODO: log
            if (parent_was_stem != parent_is_stem) {
                parent_data.is_stem = parent_is_stem;
                if (output_) {
                    output_->updateStatus(parent_index, tic_, repro_, attempt_, parent_is_stem, parent_data.is_active);
                }
            }

            // NOTE: Ordering wrt parent updates is important, this can invalidate references
            addCell(child_position, parent_index, child_is_stem);

            reproduced = true;
            break;
        }
    }

    if (!reproduced) {
        // The cell has no available space to reproduce
        parent_data.is_active = false;
        if (output_) {
            output_->updateStatus(parent_index, tic_, repro_, attempt_, parent_data.is_stem, parent_data.is_active);
        }
    }
}

int Culture::simulate(int time_steps)
{
    for (tic_ = 1; tic_ <= time_steps; ++tic_) {

        auto active_pred = [](auto tuple) -> bool {
            auto const [index, data] = tuple;
            return data.is_active;
        };

        auto get_index_lambda = [](auto tuple) {
            auto const [index, _] = tuple;
            return index;
        };

        auto active_indices_view = cell_data_ | std::views::enumerate | std::views::filter(active_pred) | std::views::transform(get_index_lambda);

        std::vector<int> shuffled_indices;
        std::copy(active_indices_view.begin(), active_indices_view.end(), std::back_inserter(shuffled_indices));
        std::shuffle(shuffled_indices.begin(), shuffled_indices.end(), rng_);

        repro_ = 0;
        for (const auto index : shuffled_indices) {
            reproduce(index);
            printf("t:%d/%d r:%d/%lu\n", tic_, time_steps, repro_, shuffled_indices.size());
            repro_ += 1;
        }
    }
    return cell_data_.size();
}