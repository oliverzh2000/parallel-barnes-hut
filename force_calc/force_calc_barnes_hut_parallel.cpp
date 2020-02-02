//
// Created by Oliver Zhang on 2020-01-12.
//

#include "force_calc_barnes_hut_parallel.h"

#include <thread>

#include "../utils/stopwatch.h"
#include "flat_oct_tree.h"

ForceCalcBarnesHutParallel::ForceCalcBarnesHutParallel(double gravConst, double softening, double theta)
        : ForceCalcBarnesHut(gravConst, softening, theta) {}

void ForceCalcBarnesHutParallel::updateNetAccel(Model &model) const {
    int hwThreadCount = std::thread::hardware_concurrency();

    auto s1 = Stopwatch::createAndStart("  acc = 0");
    for (int i = 0; i < model.size(); ++i) {
        model.acc(i) = {0, 0, 0};
    }
    s1.stopAndOutput();

//    auto s2 = Stopwatch::createAndStart("  build tree");
//    OctTree octTree{model};
//    s2.stopAndOutput();
//
//    auto s3 = Stopwatch::createAndStart("  tree traversal");
//    std::thread threads[hwThreadCount];
//    for (int i = 0; i < hwThreadCount; ++i) {
//        int start = model.size() * i / hwThreadCount;
//        int end = model.size() * (i + 1) / hwThreadCount;
//        threads[i] = std::thread{[&](int start, int end) {
//            for (int i = start; i < end; ++i) {
//                model.acc(i) += ForceCalcBarnesHut::gravFieldViaTree(octTree.root, octTree.length, model.pos(i));
//            }
//        }, start, end};
//    }

    auto s2 = Stopwatch::createAndStart("  build tree");
    FlatOctTree octTree{model};
    s2.stopAndOutput();

    auto s3 = Stopwatch::createAndStart("  tree traversal");
    std::thread threads[hwThreadCount];
    for (int i = 0; i < hwThreadCount; ++i) {
        int start = model.size() * i / hwThreadCount;
        int end = model.size() * (i + 1) / hwThreadCount;
        threads[i] = std::thread{[&](int start, int end) {
            for (int i = start; i < end; ++i) {
                model.acc(i) += ForceCalcBarnesHut::gravFieldViaTree2(octTree, FlatOctTree::root, octTree.getLength(), model.pos(i));
            }
        }, start, end};
    }

    for (auto &octantThread : threads) {
        octantThread.join();
    }
    s3.stopAndOutput();
}
