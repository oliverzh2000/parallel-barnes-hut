//
// Created by Oliver Zhang on 2020-01-01.
//

#include <limits>
#include <algorithm>

#include "oct_tree.h"

OctTree::OctTree(const Model &model) {
    // Compute the center and length first.
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
    center = minCorner * 0.5 + maxCorner * 0.5;
    length = std::max({(center - minCorner).x, (center - minCorner).y, (center - minCorner).z});

    // Insert all the positions and masses into the root tree node.
    for (const Star &star: model.getStars()) {
        root.addChild(center, length, star.pos, star.mass);
    }
}

OctTree::Node::Node(const Vec3D &centerOfMass, double mass)
        : children(), centerOfMass{centerOfMass}, totalMass{mass} {}

OctTree::Node::~Node() {
    for (auto &i : children) {
        delete i;
    }
}

void OctTree::Node::addChild(const Vec3D &center, double length, Vec3D pos, double mass) {
//    assert (isInBounds(center, length, pos));
    int octant = getOctant(center, pos);
    if (isEmpty()) {
        children[octant] = new Node{pos, mass};
        centerOfMass = (centerOfMass * totalMass + pos * mass) * (1 / (totalMass + mass));
        totalMass += mass;
        return;
    } else if (isLeaf()) {
        children[getOctant(center, centerOfMass)] = new Node{centerOfMass, totalMass};
    }
    if (children[octant] == nullptr) {
        children[octant] = new Node{pos, mass};
    } else {
        children[octant]->addChild(centerOfChildOctant(center, length, pos), length / 2, pos, mass);
    }
    centerOfMass = (centerOfMass * totalMass + pos * mass) * (1 / (totalMass + mass));
    totalMass += mass;
}

int OctTree::Node::getOctant(const Vec3D &center, const Vec3D &pos) const {
    int octant = 0;
    if (pos.x < center.x) octant += 1;
    if (pos.y < center.y) octant += 2;
    if (pos.z > center.z) octant += 4;
    return octant;
}

Vec3D OctTree::Node::centerOfChildOctant(const Vec3D &currentCenter, double currentLength, const Vec3D &pos) const {
    Vec3D newCenter = currentCenter;
    newCenter.x += (pos.x < currentCenter.x ? -0.5 : 0.5) * currentLength;
    newCenter.y += (pos.y < currentCenter.y ? -0.5 : 0.5) * currentLength;
    newCenter.z += (pos.z < currentCenter.z ? -0.5 : 0.5) * currentLength;
    return newCenter;
}

bool OctTree::Node::isInBounds(const Vec3D &center, double length, const Vec3D &pos) const {
    bool a = center.x - length <= pos.x && pos.x <= center.x + length;
    bool b = center.y - length <= pos.y && pos.y <= center.y + length;
    bool c = center.z - length <= pos.z && pos.z <= center.z + length;
    return a && b && c;
}

bool OctTree::Node::isEmpty() const {
    return totalMass == 0;
}

bool OctTree::Node::isLeaf() const {
    // TODO: potential speedup.
    // A boolean flag can be stored instead of linear time traversal through all children
    for (auto i : children) {
        if (i != nullptr) {
            return false;
        }
    }
    return true;
}

