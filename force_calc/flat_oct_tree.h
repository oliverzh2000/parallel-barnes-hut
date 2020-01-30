//
// Created by Oliver Zhang on 2020-01-25.
//

#ifndef BH_SIM_FLAT_OCT_TREE_H
#define BH_SIM_FLAT_OCT_TREE_H

#include "oct_tree.h"

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
class FlatOctTree {
    Vec3D center;
    double length; // distance from center to any side of the OctTree bounding cube.

    std::vector<Vec3D> centerOfMasses;
    std::vector<double> totalMasses;
    std::vector<std::array<int, 10>> childrenIndices;

public:
    static constexpr int root = 0;

    explicit FlatOctTree(const Model &model);

    const Vec3D &getCenter() const;

    double getLength() const;

    const Vec3D &centerOfMass(int index) const;

    double totalMass(int index) const;

    const std::array<int , 10> &children(int index) const;

    bool isEmpty(int index) const;

    bool isLeaf(int index) const;

private:
    struct BFSNode {
        const OctTree::Node &node;
        int parentIndex = -1;
        char octantIndex = -1;
    };

    void addChild(int node, const Vec3D &center, double length, const Vec3D &pos, double mass);

    /// Returns the index of the newly created child node.
    int newNodeAtEnd(const Vec3D &pos, double mass);

    /// Return a int between 0 and 7 that uniquely identifies
    /// the octant that the given position is, relative to the given center.
    static int getOctant(const Vec3D &center, const Vec3D &pos);

    /// Compute the center of the child octant Node that contains the given position.
    static Vec3D centerOfChildOctant(const Vec3D &currentCenter, double currentLength, const Vec3D &pos);

    static bool isInBounds(const Vec3D &center, double length, const Vec3D &pos);

    static std::tuple<Vec3D, double> getBoundingBox(const Model &model);
};


#endif //BH_SIM_FLAT_OCT_TREE_H
