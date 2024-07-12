#include "culture.hpp"
#include "output.hpp"

#include <cxxopts.hpp>

#include <cstdio>
#include <format>
#include <iostream>
#include <string>

static void run(float prob_stem, float prob_diff, int realization, int steps)
{
    std::string run_name = std::format("ps{}_pd{}_r{}", prob_stem, prob_diff, realization);
    Output out(run_name);
    Culture culture(1.0f, &out);
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
        ("rng-seed", "Random number generator seed", cxxopts::value<int>())
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

#pragma omp parallel
#pragma omp single
    for (const auto ps : pstems) {
        for (const auto pd : pdiffs) {
            for (int r = 0; r < realizations; ++r) {
#pragma omp task
                run(ps, pd, r, steps_per_realization);
            }
        }
    }
    return 0;
}