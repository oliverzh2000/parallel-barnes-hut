//
// Created by Oliver Zhang on 2020-01-02.
//

#include "force_calc_barnes_hut.h"
#include "oct_tree.h"

ForceCalcBarnesHut::ForceCalcBarnesHut(double gravConst, double softening, double theta)
: ForceCalc(gravConst, softening), theta{theta} {}

void ForceCalcBarnesHut::updateNetAccel(Model &model) const {
    for (int i = 0; i < model.size(); ++i) {
        model.acc(i) = {0, 0, 0};
    }
    OctTree octTree{model};
    for (int i = 0; i < model.size(); ++i) {
        model.acc(i) += gravFieldViaTree(octTree.root, octTree.length, model.pos(i));
    }
}

Vec3D ForceCalcBarnesHut::gravFieldViaTree(const OctTree::Node &node, double length, const Vec3D &pos) const {
    // Calculate force directly when:
    // 1) On leaf nodes, since there are no more children.
    // 2) Angular separation of this node's center of mass and pos is less than max threshold.
    if (node.isLeaf() || node.centerOfMass.distanceTo(pos) / length < theta) {
        return gravFieldDueToSingleObject(node.centerOfMass, node.totalMass, pos);
    } else {
        Vec3D accSum;
        for (const OctTree::Node *child : node.children) {
            if (child != nullptr) {
                accSum += gravFieldViaTree(*child, length * 0.5, pos);
            }
        }
        return accSum;
    }
}
