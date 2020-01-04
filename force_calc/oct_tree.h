//
// Created by Oliver Zhang on 2020-01-01.
//

#ifndef BH_SIM_OCTTREE_H
#define BH_SIM_OCTTREE_H

#include <vector>
#include "../vec3d.h"
#include "../star.h"
#include "../model.h"

/**
 * OctTree data structure to enable efficient force computation with Barnes-Hut algorithm
 *
 * Space partitioning is done in cubes to save memory (with all subtrees also being cubes).
 */
class OctTree final {
    Vec3D center;
    double length; // distance from center to any side of the OctTree bounding cube.

    class Node final {
        Node *children[8];
        Vec3D centerOfMass;
        double totalMass;

    public:
        /// Construct a new Node with one child located at the given center and given mass.
        explicit Node(const Vec3D &centerOfMass={0, 0, 0}, double mass=0);

        /**
         * Recursively add a child into this Node.
         *
         * The center of mass and total mass of this Node is appropriately updated.
         * @param center This Node's center
         * @param length This Node's distance from center to any side of the bounding cube
         * @param pos Position of the child to be added
         * @param mass Mass of the child to be added
         */
        void addChild(const Vec3D &center, double length, Vec3D pos, double mass);

        /// A node is considered empty when total mass is 0.
        /// Note: all empty Nodes are leaves.
        bool isEmpty() const;

        /// A node is considered leaf if all children are nullptr.
        bool isLeaf() const;

        friend class ForceCalcBarnesHut;
    private:
        /// Return a int between 0 and 7 that uniquely identifies
        /// the octant that the given position is, relative to the given center.
        int getOctant(const Vec3D &center, const Vec3D &pos) const;

        /// Compute the center of the child octant Node that contains the given position.
        Vec3D centerOfChildOctant(const Vec3D &currentCenter, double currentLength, const Vec3D &pos) const;

        bool isInBounds(const Vec3D &center, double length, const Vec3D &pos) const;
    };

    Node root = Node{Vec3D{}};

public:
    explicit OctTree(const Model &model);

    friend class ForceCalcBarnesHut;
};


#endif //BH_SIM_OCTTREE_H
