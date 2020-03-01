//
// Created by Oliver Zhang on 2020-01-25.
//

#ifndef BH_SIM_BREADTH_FIRST_OCTREE_H
#define BH_SIM_BREADTH_FIRST_OCTREE_H

#include "octree.h"

#include <vector>
#include <array>
#include "../base/vec3d.h"
#include "../base/star.h"
#include "../base/model.h"

/**
 * Oct-tree data structure to enable efficient force computation with Barnes-Hut algorithm
 *
 * Space partitioning is done in cubes to save memory (with all subtrees also being cubes).
 *
 * To reduce the pointer chasing codes, the entire tree is flattened in breadth-first order
 * and implemented as an array (or array of fields).
 */
class BreadthFirstOctree {
    Vec3D center;
    double length; // distance from center to any side of the OctTree bounding cube.

    std::vector<Vec3D> centerOfMasses;
    std::vector<double> totalMasses;

    // Since the tree is stored in a breadth-first ordering, we can guarantee that the children of each node are stored contiguously, and in order.
    // This means that only the index of the first child needs to be stored.
    std::vector<int> firstChildIndices;
    std::vector<uint8_t> childrenCounts;

public:
    static constexpr int root = 0;

    explicit BreadthFirstOctree(const Model &model);

    double getLength() const;

    const Vec3D &centerOfMass(int index) const;

    double totalMass(int index) const;

    int firstChildIndex(int index) const;

    uint8_t childrenCount(int index) const;

    bool isLeaf(int index) const;

private:
    struct BFSNode {
        const Octree::Node &node;
        int parentIndex = -1;
        bool isFirstChild = false;
    };

    /// Returns the index of the newly created child node.
    int newNodeAtEnd(const Vec3D &pos, double mass);
};


#endif //BH_SIM_BREADTH_FIRST_OCTREE_H
