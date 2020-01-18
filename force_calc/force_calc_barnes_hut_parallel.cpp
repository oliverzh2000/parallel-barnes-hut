//
// Created by Oliver Zhang on 2020-01-12.
//

#include <thread>
#include <tuple>
#include <chrono>
#include "force_calc_barnes_hut_parallel.h"

ForceCalcBarnesHutParallel::ForceCalcBarnesHutParallel(double gravConst, double softening, double theta)
        : ForceCalcBarnesHut(gravConst, softening, theta) {}

void ForceCalcBarnesHutParallel::updateNetAccel(Model &model) const {
    for (int i = 0; i < model.size(); ++i) {
        model.acc(i) = {0, 0, 0};
    }
    OctTree octTree{model};

    std::cout << "done tree" << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds{100});

    // If running on 8 core CPU:
    // then partition space into 8 octants and compute acc on each octant with its own thread.
    // TODO: support arbitrary number of cores.
    if (std::thread::hardware_concurrency() != 8) {
        throw std::runtime_error{"Must have 8 hardware threads."};
    }

    // To save memory, store the indices (in model) of the stars in each octant,
    // instead of copying them into 8 brand new models.

    // Possible way to partition:
    // - split 8 way normally (node::getoctant), then split the top 8 in half.

    int numThreads = 8;
//    std::vector<int> octantStarIndices[numThreads];
//    auto[center, length] = OctTree::getBoundingBox(model);
//    bool placeInFirstHalfOctant = false;
//    for (int i = 0; i < model.size(); ++i) {
//        if (placeInFirstHalfOctant) {
//            octantStarIndices[OctTree::Node::getOctant(center, model.pos(i))].emplace_back(i);
//        } else {
//            octantStarIndices[OctTree::Node::getOctant(center, model.pos(i)) + 8].emplace_back(i);
//        }
//        placeInFirstHalfOctant = !placeInFirstHalfOctant;
//    }
//
//    int sum = 0;
//    for (int i = 0; i < numThreads; ++i) {
//        std::cout << "octantStarIndices[i].size(): " << octantStarIndices[i].size() << std::endl;
//        sum += octantStarIndices[i].size();
//    }
//    std::cout << "sum: " << sum << std::endl;

    std::thread octantThreads[numThreads];
    for (int i = 0; i < numThreads; ++i) {
        int start = i * (model.size() / numThreads);
        int end = start + (model.size() / numThreads);
        octantThreads[i] = std::thread{&ForceCalcBarnesHutParallel::updateNetAccelForIndices2, this, std::ref(model), std::ref(octTree), start, end};
    }
    for (std::thread &octantThread : octantThreads) {
        octantThread.join();
    }
}

void ForceCalcBarnesHutParallel::updateNetAccelForIndices(Model &model, const OctTree &octTree, const std::vector<int> indices) const {
    for (int i: indices) {
        model.acc(i) += ForceCalcBarnesHut::gravFieldViaTree(octTree.root, octTree.length, model.pos(i));
    }
}

void ForceCalcBarnesHutParallel::updateNetAccelForIndices2(Model &model, const OctTree &octTree, int start, int end) const {
    for (int i = start; i < end; ++i) {
        model.acc(i) += ForceCalcBarnesHut::gravFieldViaTree(octTree.root, octTree.length, model.pos(i));
    }
}

