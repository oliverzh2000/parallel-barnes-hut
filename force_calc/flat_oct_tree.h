//
// Created by Oliver Zhang on 2020-01-25.
//

#ifndef BH_SIM_ARRAY_OCT_TREE_H
#define BH_SIM_ARRAY_OCT_TREE_H

#include "oct_tree.h"

#include <vector>
#include "../base/vec3d.h"
#include "../base/star.h"
#include "../base/model.h"

/**
 * ArrayOctTree data structure to enable efficient force computation with Barnes-Hut algorithm
 *
 * Space partitioning is done in cubes to save memory (with all subtrees also being cubes).
 *
 * To reduce the pointer chasing codes, the entire tree is implemented as an array,
 * (or array of fields).
 */
class ArrayOctTree {
    Vec3D center;
    double length; // distance from center to any side of the OctTree bounding cube.
    std::vector<Vec3D> centerOfMasses;
    std::vector<double> totalMasses;
    std::vector<int[8]> childrens;

    static constexpr int root = 0;

    /**
     * Wrapper around an index in the parent ArrayOctTree.
     */
    class Node final {
        int index = -1;

    public:
        /// Construct a new Node that refers to the information stored in the parent ArrayOctTree at the given index.
        explicit Node(int index);

        /// Recursively add a child into the parent ArrayOctTree starting from this Node.
        void addChild(const Vec3D &other, double length, Vec3D pos, double mass);

        /// A node is considered empty when total mass is 0.
        /// Note: If all star masses are positive, then all empty Nodes are also leaves.
        bool isEmpty() const;

        /// A node is considered leaf if has no children (aka all children point to index -1).
        bool isLeaf() const;
    };


public:
    explicit ArrayOctTree(const Model &model);

    const Vec3D &centerOfMass(int index) const;

    double totalMass(int index) const;

    const int *children(int index) const;

    bool isEmpty(int index) const;

    bool isLeaf(int index) const;

private:
    void addChild(int node, const Vec3D &center, double length, const Vec3D &pos, double mass);

    int addChildToEnd()

    /// Return a int between 0 and 7 that uniquely identifies
    /// the octant that the given position is, relative to the given center.
    static int getOctant(const Vec3D &center, const Vec3D &pos);
    /// Compute the center of the child octant Node that contains the given position.
    static Vec3D centerOfChildOctant(const Vec3D &currentCenter, double currentLength, const Vec3D &pos);

    static bool isInBounds(const Vec3D &center, double length, const Vec3D &pos);

    static std::tuple<Vec3D, double> getBoundingBox(const Model &model);
};


#endif //BH_SIM_ARRAY_OCT_TREE_H
