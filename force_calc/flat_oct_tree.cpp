//

#include "flat_oct_tree.h"

#include <algorithm>
#include <tuple>
#include <queue>

#include "oct_tree.h"
#include "../utils/stopwatch.h"

FlatOctTree::FlatOctTree(const Model &model) {
    // Construct a normal oct-tree then flatten it breadth-first.

    auto s1 = Stopwatch::createAndStart("    build oct-tree");
    OctTree octTree{model};
    center = octTree.center;
    length = octTree.length;
    s1.stopAndOutput();

    auto s2 = Stopwatch::createAndStart("    flatten oct-tree");
    // insert octTree.root into queue, with parent and octant indices both set to 0.
    // while queue not empty:
    //   insert front node
    //   set the front node's parent's children[i] to point to the front node, where i is front node's octant
    //   for each ith child of front node:
    //     add child to queue, with parent index set to front node, and octant index set to i.
    std::queue<BFSNode> nodes;
    nodes.push({octTree.root, -1, -1});

    while (!nodes.empty()) {
        const BFSNode &front = nodes.front();
        int frontIndex = newNodeAtEnd(front.node.centerOfMass, front.node.totalMass);
        if (front.parentIndex != -1 && front.octantIndex != -1) {
            childrenIndices.at(front.parentIndex).at(front.octantIndex) = frontIndex;
        }
        if (!front.node.isLeaf()) {
            for (char i = 0; i < 8; ++i) {
                if (front.node.children[i] != nullptr) {
                    nodes.emplace(BFSNode{*front.node.children[i], frontIndex, i});
                }
            }
        }
        nodes.pop();
    }
    s2.stopAndOutput();
}

void FlatOctTree::addChild(int node, const Vec3D &center, double length, const Vec3D &pos, double mass) {
    int octant = getOctant(center, pos);
    if (isEmpty(node)) {
        childrenIndices.at(node).at(octant) = newNodeAtEnd(pos, mass);
        centerOfMasses.at(node) = pos;
        totalMasses.at(node) = mass;
        return;
    } else if (isLeaf(node)) {
        childrenIndices.at(node).at(getOctant(center, centerOfMass(node))) =
                newNodeAtEnd(centerOfMass(node), totalMass(node));
    }
    if (childrenIndices.at(node).at(octant) == -1) {
        childrenIndices.at(node).at(octant) = newNodeAtEnd(pos, mass);
    } else {

    }
}

int FlatOctTree::newNodeAtEnd(const Vec3D &pos, double mass) {
    int newIndex = centerOfMasses.size();
    centerOfMasses.emplace_back(pos);
    totalMasses.emplace_back(mass);
    childrenIndices.emplace_back(std::array<int, 10>());
    childrenIndices.at(newIndex).fill(-1);
    return newIndex;
}

const Vec3D &FlatOctTree::getCenter() const {
    return center;
}

//
// Created by Oliver Zhang on 2020-01-25.
double FlatOctTree::getLength() const {
    return length;
}

const Vec3D &FlatOctTree::centerOfMass(int index) const {
    return centerOfMasses.at(index);
}

double FlatOctTree::totalMass(int index) const {
    return totalMasses.at(index);
}

const std::array<int, 10> &FlatOctTree::children(int index) const {
    return childrenIndices.at(index);
}

bool FlatOctTree::isEmpty(int index) const {
    return totalMasses.at(index) == 0;
}

bool FlatOctTree::isLeaf(int index) const {
    for (int i = 0; i < 8; ++i) {
        if (childrenIndices.at(index)[i] != -1) {
            return false;
        }
    }
    return true;
}

int FlatOctTree::getOctant(const Vec3D &center, const Vec3D &pos) {
    int octant = 0;
    if (pos.x < center.x) octant += 1;
    if (pos.y < center.y) octant += 2;
    if (pos.z > center.z) octant += 4;
    return octant;
}

Vec3D FlatOctTree::centerOfChildOctant(const Vec3D &currentCenter, double currentLength, const Vec3D &pos) {
    Vec3D newCenter = currentCenter;
    newCenter.x += (pos.x < currentCenter.x ? -0.5 : 0.5) * currentLength;
    newCenter.y += (pos.y < currentCenter.y ? -0.5 : 0.5) * currentLength;
    newCenter.z += (pos.z < currentCenter.z ? -0.5 : 0.5) * currentLength;
    return newCenter;
}

bool FlatOctTree::isInBounds(const Vec3D &center, double length, const Vec3D &pos) {
    bool a = center.x - length <= pos.x && pos.x <= center.x + length;
    bool b = center.y - length <= pos.y && pos.y <= center.y + length;
    bool c = center.z - length <= pos.z && pos.z <= center.z + length;
    return a && b && c;
}

std::tuple<Vec3D, double> FlatOctTree::getBoundingBox(const Model &model) {
    constexpr double minDouble = std::numeric_limits<double>::lowest();
    constexpr double maxDouble = std::numeric_limits<double>::max();
    Vec3D minCorner = {maxDouble, maxDouble, maxDouble};
    Vec3D maxCorner = {minDouble, minDouble, minDouble};
    for (const Star &star: model.getStars()) {
        if (star.pos.x < minCorner.x) minCorner.x = star.pos.x;
        if (star.pos.y < minCorner.y) minCorner.y = star.pos.y;
        if (star.pos.z < minCorner.z) minCorner.z = star.pos.z;

        if (star.pos.x > maxCorner.x) maxCorner.x = star.pos.x;
        if (star.pos.y > maxCorner.y) maxCorner.y = star.pos.y;
        if (star.pos.z > maxCorner.z) maxCorner.z = star.pos.z;
    }
    Vec3D center = minCorner * 0.5 + maxCorner * 0.5;
    double length = std::max({(center - minCorner).x, (center - minCorner).y, (center - minCorner).z});
    return {center, length};
}
