//
// Created by Oliver Zhang on 2020-01-02.
//

#include "force_calc_barnes_hut.h"

#include "octree.h"
#include "breadth_first_octree.h"

#include "../utils/stopwatch.h"

ForceCalcBarnesHut::ForceCalcBarnesHut(double gravConst, double softening, double theta)
        : ForceCalc(gravConst, softening), theta{theta} {}

void ForceCalcBarnesHut::updateNetAccel(Model &model) const {
    auto s1 = Stopwatch::createAndStart("acc = 0");
    for (int i = 0; i < model.size(); ++i) {
        model.acc(i) = {0, 0, 0};
    }
    s1.stopAndOutput();

    bool flat = false;

    if (!flat) {
        auto s2 = Stopwatch::createAndStart("build octree");
        Octree octTree{model};
        s2.stopAndOutput();

        auto s3 = Stopwatch::createAndStart("traverse octree");
        for (int i = 0; i < model.size(); ++i) {
            model.acc(i) += gravFieldViaTree(octTree.root, octTree.length, model.pos(i));
        }

        s3.stopAndOutput();
    } else {
        auto s2 = Stopwatch::createAndStart("build octree");
        BreadthFirstOctree octTree{model};
        s2.stopAndOutput();

        auto s3 = Stopwatch::createAndStart("traverse octree");
        for (int i = 0; i < model.size(); ++i) {
            model.acc(i) += gravFieldViaTree2(octTree, BreadthFirstOctree::root, octTree.getLength(), model.pos(i));
        }
        s3.stopAndOutput();
    }
}

Vec3D ForceCalcBarnesHut::gravFieldViaTree(const Octree::Node &node, double length, const Vec3D &pos) const {
    // Calculate force directly when:
    // 1) On leaf nodes, since there are no more children.
    // 2) Angular separation of this node's center of mass and pos is less than max threshold.
    double ratio = length / node.centerOfMass.distanceTo(pos);
    if (node.isLeaf() || ratio < theta) {
        return gravFieldDueToSingleObject(node.centerOfMass, node.totalMass, pos);
    } else {
        Vec3D accSum;
        for (int i = 0; i < 8; ++i) {
            if (node.children[i] != nullptr) {
                accSum += gravFieldViaTree(*node.children[i], length * 0.5, pos);
            }
        }
        return accSum;
    }
}

Vec3D ForceCalcBarnesHut::gravFieldViaTree2(const BreadthFirstOctree &tree, int node, double length, const Vec3D &pos) const {
    // Calculate force directly when:
    // 1) On leaf nodes, since there are no more children.
    // 2) Angular separation of this node's center of mass and pos is less than max threshold.
    double ratio = length / tree.centerOfMass(node).distanceTo(pos);
    if (tree.isLeaf(node) || ratio < theta) {
        return gravFieldDueToSingleObject(tree.centerOfMass(node), tree.totalMass(node), pos);
    } else {
        Vec3D accSum;
        for (int child = tree.firstChildIndex(node); child < tree.firstChildIndex(node) + tree.childrenCount(node); ++child) {
            accSum += gravFieldViaTree2(tree, child, length * 0.5, pos);
        }
        return accSum;
    }
}
