//
// Created by Oliver Zhang on 2020-01-01.
//

#ifndef BH_SIM_OCTTREE_H
#define BH_SIM_OCTTREE_H

#include <vector>

#include "../base/vec3d.h"
#include "../base/star.h"
#include "../base/model.h"

/**
 * Oct-tree data structure to enable efficient force computation with Barnes-Hut algorithm
 *
 * Space partitioning is done in cubes to save memory (with all subtrees also being cubes).
 */
class Octree {
    Vec3D center = {0, 0, 0};
    double length = 0; // Side length of the Octree bounding cube.

public:
    class Node final {
        Node *children[8];
        Vec3D centerOfMass;
        double totalMass;

    public:
        /// Construct a new Node with one child located at the given center and given mass.
        explicit Node(const Vec3D &centerOfMass = {0, 0, 0}, double mass = 0);

        ~Node();

        /**
         * Recursively add a child into this Node.
         *
         * The center of mass and total mass of this Node is appropriately updated.
         * @param center This Node's center
         * @param length This Node's distance from center to any side of the bounding cube
         * @param pos Position of the child to be added
         * @param mass Mass of the child to be added
         */
        void addChild(const Vec3D &center, double length, const Vec3D &pos, double mass);

        /// A node is considered empty when total mass is 0.
        /// Note: If all star masses are positive, then all empty Nodes are also leaves.
        bool isEmpty() const;

        /// A node is considered leaf if it has no children (aka all children are nullptr).
        bool isLeaf() const;

        void debugPrint(int depth) const;

        friend class ForceCalcBarnesHut;
        friend class ForceCalcBarnesHutParallel;
        friend class OctTree;
        friend class BreadthFirstOctree;
    };

    Node root = Node{Vec3D{}};

public:
    explicit Octree(const Model &model);

    Octree(const Model &model, const Vec3D &center, double length);

    void debugPrint() const;

    friend class ForceCalcBarnesHut;

    friend class ForceCalcBarnesHutParallel;
    friend class BreadthFirstOctree;

private:
    void initOctants(const Model &model, const Vec3D &center, double length);

    /// Return a int between 0 and 7 that uniquely identifies
    /// the octant that the given position is, relative to the given center.
    static uint8_t getOctant(const Vec3D &center, const Vec3D &pos);

    /// Compute the center of the given octant.
    static Vec3D centerOfOctant(const Vec3D &currentCenter, double currentLength, uint8_t octant);

    /// Compute the center of the octant that contains the given position.
    static Vec3D centerOfOctant(const Vec3D &currentCenter, double currentLength, const Vec3D &pos);

    static bool isInBounds(const Vec3D &center, double length, const Vec3D &pos);
};


#endif //BH_SIM_OCTTREE_H
