#include "culture.hpp"
#include "output.hpp"

#include "pcg_random.hpp"
#include <cxxopts.hpp>

#include <cstdio>
#include <format>
#include <iostream>
#include <string>
#include <vector>

static void run(float prob_stem, float prob_diff, uint64_t seed, int steps)
{
    std::string run_name = std::format("ps{}_pd{}_seed{}", prob_stem, prob_diff, seed);
    Output out(run_name);
    Culture culture(1.0f, &out, seed);
    culture.addCell(Position { 0.0f, 0.0f, 0.0f }, 0, true);
    culture.prob_diff = prob_stem;
    culture.prob_stem = prob_diff;
    culture.simulate(steps);
}

int main(int argc, char** argv)
{
    cxxopts::Options options("tumorsphere", "Simulates tumorsphere growth with the indicated parameters.");

    // clang-format off
    options.add_options()
        ("prob-stem", "List of probabilities of stem cells", cxxopts::value<std::vector<float>>())
        ("prob-diff", "List of probabilities of differentiated cells", cxxopts::value<std::vector<float>>())
        ("realizations", "Number of realizations", cxxopts::value<int>())
        ("steps-per-realization", "Number of steps per realization", cxxopts::value<int>())
        ("rng-seed", "Random number generator seed", cxxopts::value<uint64_t>())
        ("h,help", "Print usage");
    // clang-format on

    auto options_result = options.parse(argc, argv);

    if (options_result.count("help") > 0) {
        std::cout << options.help() << std::endl;
        exit(0);
    }

    auto pstems = options_result["prob-stem"].as<std::vector<float>>();
    auto pdiffs = options_result["prob-diff"].as<std::vector<float>>();
    auto realizations = options_result["realizations"].as<int>();
    auto steps_per_realization = options_result["steps-per-realization"].as<int>();

    std::vector<uint64_t> seeds;
    if (realizations == 1) {
        seeds.push_back(options_result["rng-seed"].as<uint64_t>());
    } else {
        pcg32 rng(options_result["rng-seed"].as<uint64_t>());
        std::uniform_int_distribution<uint64_t> uniform_dist(2ull << 20, 2ull << 50);
        for (int r = 0; r < realizations; ++r) {
            seeds.push_back(uniform_dist(rng));
        }
    }

#pragma omp parallel shared(seeds, steps_per_realization)
#pragma omp single
    for (const auto ps : pstems) {
        for (const auto pd : pdiffs) {
            for (int r = 0; r < realizations; ++r) {
#pragma omp task
                run(ps, pd, seeds[r], steps_per_realization);
            }
        }
    }
    return 0;
}