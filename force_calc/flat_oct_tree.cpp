//
// Created by Oliver Zhang on 2020-01-25.
//

#include "flat_oct_tree.h"

#include <algorithm>
#include <tuple>
#include <queue>

#include "oct_tree.h"
#include "../utils/stopwatch.h"

FlatOctTree::FlatOctTree(const Model &model) {
    // Construct a normal oct-tree then flatten it breadth-first.

    auto s1 = Stopwatch::createAndStart("build oct-tree");
    OctTree octTree{model};
    center = octTree.center;
    length = octTree.length;
    s1.stopAndOutput();

    auto s2 = Stopwatch::createAndStart("flatten oct-tree");
    // insert octTree.root into queue, with parent and octant indices both set to 0.
    // while queue not empty:
    //   insert front node
    //   set the front node's parent's children[i] to point to the front node, where i is front node's octant
    //   for each ith child of front node:
    //     add child to queue, with parent index set to front node, and octant index set to i.
    std::queue<BFSNode> nodes;

//    nodes.push({octTree.root, -1, -1});
//    while (!nodes.empty()) {
//        const BFSNode &front = nodes.front();
//        int frontIndex = newNodeAtEnd(front.node.centerOfMass, front.node.totalMass);
//        if (front.parentIndex != -1 && front.octantIndex != -1) {
//            firstChildIndices.at(front.parentIndex).at(front.octantIndex) = frontIndex;
//        }
//        if (!front.node.isLeaf()) {
//            for (char i = 0; i < 8; ++i) {
//                if (front.node.children[i] != nullptr) {
//                    nodes.emplace(BFSNode{*front.node.children[i], frontIndex, i});
//                }
//            }
//        }
//        nodes.pop();
//    }

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
            for (uint8_t i = 0; i < 8; i++) {
                if (front.node.children[i] != nullptr) {
                    nodes.emplace(BFSNode{*front.node.children[i], frontIndex, isFirstChild});
                    isFirstChild = false;
                }
            }
        }
        nodes.pop();
    }
    s2.stopAndOutput();
}

int FlatOctTree::newNodeAtEnd(const Vec3D &pos, double mass) {
    int newIndex = centerOfMasses.size();
    centerOfMasses.emplace_back(pos);
    totalMasses.emplace_back(mass);
    firstChildIndices.emplace_back(-1);
    childrenCounts.emplace_back(0);
    return newIndex;
}

double FlatOctTree::getLength() const {
    return length;
}

const Vec3D &FlatOctTree::centerOfMass(int index) const {
    return centerOfMasses.at(index);
}

double FlatOctTree::totalMass(int index) const {
    return totalMasses.at(index);
}

int FlatOctTree::firstChildIndex(int index) const {
    return firstChildIndices.at(index);
}

uint8_t FlatOctTree::childrenCount(int index) const {
    return childrenCounts.at(index);
}

bool FlatOctTree::isLeaf(int index) const {
    return childrenCounts.at(index) == 0;
}

