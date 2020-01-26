//
// Created by Oliver Zhang on 2020-01-25.
//

#include "array_oct_tree.h"

#include <algorithm>
#include <tuple>

ArrayOctTree::ArrayOctTree(const Model &model) {
    auto [center, length] = getBoundingBox(model);
    this->center = center;
    this->length = length;

    // Insert all the positions and masses into the root tree node.
    for (const Star &star: model.getStars()) {
        addChild(root, center, length, star.pos, star.mass);
    }
}

void ArrayOctTree::addChild(int node, const Vec3D &center, double length, const Vec3D &pos, double mass) {
    int octant = getOctant(center, pos);
    if (isEmpty(node)) {
        children(node) =
    }
}

const Vec3D &ArrayOctTree::centerOfMass(int index) const {
    return centerOfMasses.at(index);
}

double ArrayOctTree::totalMass(int index) const {
    return totalMasses.at(index);
}

const int *ArrayOctTree::children(int index) const {
    return childrens.at(index);
}

bool ArrayOctTree::isEmpty(int index) const {
    return totalMasses.at(index) == 0;
}

bool ArrayOctTree::isLeaf(int index) const {
    for (int i = 0; i < 8; ++i) {
        if (childrens.at(index)[i] != -1) {
            return true;
        }
    }
    return false;
}

int ArrayOctTree::getOctant(const Vec3D &center, const Vec3D &pos) {
    int octant = 0;
    if (pos.x < center.x) octant += 1;
    if (pos.y < center.y) octant += 2;
    if (pos.z > center.z) octant += 4;
    return octant;
}

Vec3D ArrayOctTree::centerOfChildOctant(const Vec3D &currentCenter, double currentLength, const Vec3D &pos) {
    Vec3D newCenter = currentCenter;
    newCenter.x += (pos.x < currentCenter.x ? -0.5 : 0.5) * currentLength;
    newCenter.y += (pos.y < currentCenter.y ? -0.5 : 0.5) * currentLength;
    newCenter.z += (pos.z < currentCenter.z ? -0.5 : 0.5) * currentLength;
    return newCenter;
}

bool ArrayOctTree::isInBounds(const Vec3D &center, double length, const Vec3D &pos) {
    bool a = center.x - length <= pos.x && pos.x <= center.x + length;
    bool b = center.y - length <= pos.y && pos.y <= center.y + length;
    bool c = center.z - length <= pos.z && pos.z <= center.z + length;
    return a && b && c;
}

std::tuple<Vec3D, double> ArrayOctTree::getBoundingBox(const Model &model) {
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
