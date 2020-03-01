//
// Created by Oliver Zhang on 2020-01-25.
//

#include "breadth_first_octree.h"

#include "octree.h"
#include "utils/stopwatch.h"

#include <queue>
#include <tuple>

BreadthFirstOctree::BreadthFirstOctree(const Model &model) {
    // Construct a normal oct-tree then flatten it breadth-first.

    auto s1 = Stopwatch::createAndStart("build octree");
    Octree octTree{model};
    center = octTree.center;
    length = octTree.length;

    auto s2 = Stopwatch::createAndStart("flatten octree");
    // insert octTree.root into queue, with parent and octant indices both set to 0.
    // while queue not empty:
    //   insert front node
    //   set the front node's parent's children[i] to point to the front node, where i is front node's octant
    //   for each ith child of front node:
    //     add child to queue, with parent index set to front node, and octant index set to i.
    std::queue<BFSNode> nodes;

    nodes.emplace(BFSNode{octTree.root, -1, true});
    while (!nodes.empty()) {
        const BFSNode &front = nodes.front();
        int frontIndex = newNodeAtEnd(front.node.centerOfMass, front.node.totalMass);
        // TODO: handle the first root node as a special case to
        //  avoid this special check each time for performance, if necessary.
        if (front.parentIndex != -1) {
            if (front.isFirstChild) {
                firstChildIndices.at(front.parentIndex) = frontIndex;
            }
            childrenCounts.at(front.parentIndex)++;
        }
        if (!front.node.isLeaf()) {
            bool isFirstChild = true;
            for (const auto &child : front.node.children) {
                if (child != nullptr) {
                    nodes.emplace(BFSNode{*child, frontIndex, isFirstChild});
                    isFirstChild = false;
                }
            }
        }
        nodes.pop();
    }
    s2.stopAndOutput();
}

int BreadthFirstOctree::newNodeAtEnd(const Vec3D &pos, double mass) {
    int newIndex = centerOfMasses.size();
    centerOfMasses.emplace_back(pos);
    totalMasses.emplace_back(mass);
    firstChildIndices.emplace_back(-1);
    childrenCounts.emplace_back(0);
    return newIndex;
}

double BreadthFirstOctree::getLength() const {
    return length;
}

const Vec3D &BreadthFirstOctree::centerOfMass(int index) const {
    return centerOfMasses.at(index);
}

double BreadthFirstOctree::totalMass(int index) const {
    return totalMasses.at(index);
}

int BreadthFirstOctree::firstChildIndex(int index) const {
    return firstChildIndices.at(index);
}

uint8_t BreadthFirstOctree::childrenCount(int index) const {
    return childrenCounts.at(index);
}

bool BreadthFirstOctree::isLeaf(int index) const {
    return childrenCounts.at(index) == 0;
}
