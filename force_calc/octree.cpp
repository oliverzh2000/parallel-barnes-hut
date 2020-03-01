//
// Created by Oliver Zhang on 2020-01-01.
//

#include "octree.h"
#include <tuple>

#include <thread>
#include <limits>
#include <algorithm>
#include <assert.h>

#include "../base/model.h"

Octree::Octree(const Model &model) {
    auto [center, length] = model.boundingBox();
    this->center = center;
    this->length = length;

    int hwThreadCount = std::thread::hardware_concurrency();

    // TODO: remove when multithreading construction ready.
    hwThreadCount = 1;

    if (hwThreadCount == 8) {
//        // If there are 8 threads, let each one build a tree in a quadrant, then join them into the final root.
//        std::array<Model, 8> subModels;
//        std::array<OctTree *, 8> subTrees{nullptr};
//        for (const Star &star: model.getStars()) {
//            subModels.at(getOctant(center, star.pos)).addStar(star);
//        }
//        for (int i = 0; i < 8; ++i) {
//            if (subModels[i].size() > 0) {
//                subTrees[i]= new OctTree(subModels[i], centerOfOctant(center, length, i), length * 0.5);
//                std::cout << "i: " << i << std::endl;
//                subTrees[i]->debugPrint();
//            }
//        }
//        Vec3D centerOfMass = {0, 0, 0};
//        double totalMass = 0;
//        for (int i = 0; i < 8; ++i) {
//            if (subModels[i].size() > 0) {
//                centerOfMass += subTrees[i]->root.centerOfMass * subTrees[i]->root.totalMass;
//                totalMass += subTrees[i]->root.totalMass;
//            }
//        }
//        root = Node{centerOfMass * (1 / totalMass), totalMass};
//        root.children = new Node *[8]();
//        for (int i = 0; i < 8; ++i) {
//            if (subModels[i].size() > 0) {
//                root.children[i] = &subTrees[i]->root;
//            }
//        }
    } else {
        // Insert all the positions and masses directly into the root tree node.
        initOctants(model, center, length);
    }
}

Octree::Octree(const Model &model, const Vec3D &center, double length) {
    initOctants(model, center, length);
}

void Octree::initOctants(const Model &model, const Vec3D &center, double length) {
    for (const Star &star: model.getStars()) {
        root.addChild(center, length, star.pos, star.mass);
    }
}

Octree::Node::Node(const Vec3D &centerOfMass, double mass)
        : children(), centerOfMass{centerOfMass}, totalMass{mass} {}

Octree::Node::~Node() {
    if (!isLeaf()) {
        for (auto &child : children) {
            delete child;
        }
    }
}

void Octree::Node::addChild(const Vec3D &center, double length, const Vec3D &pos, double mass) {
    assert (isInBounds(center, length, pos));
    int octant = getOctant(center, pos);
    if (isEmpty()) {
        // This is a special case for the root node. No other nodes should be empty.
        centerOfMass = pos;
        totalMass = mass;
        return;
    } else if (isLeaf()) {
        children[getOctant(center, centerOfMass)] = new Node{centerOfMass, totalMass};
    }
    if (children[octant] == nullptr) {
        children[octant] = new Node{pos, mass};
    } else {
        children[octant]->addChild(centerOfOctant(center, length, pos), length * 0.5, pos, mass);
    }
    centerOfMass = (centerOfMass * totalMass + pos * mass) * (1 / (totalMass + mass));
    totalMass += mass;
}

bool Octree::Node::isEmpty() const {
    return totalMass == 0;
}

bool Octree::Node::isLeaf() const {
    for (const auto &child : children) {
        if (child != nullptr) {
            return false;
        }
    }
    return true;
}

void Octree::Node::debugPrint(int depth) const {
    std::cout << std::string(" ", depth * 2) << centerOfMass << ", " << totalMass << std::endl;
    if (!isLeaf()) {
        for (int i = 0; i < 8; ++i) {
            if (children[i] != nullptr) {
                std::cout << std::string(" ", depth * 2) << i << ":";
                children[i]->debugPrint(depth + 1);
            }
        }
    }
}

uint8_t Octree::getOctant(const Vec3D &center, const Vec3D &pos) {
    uint8_t octant = 0;
    if (pos.x > center.x) octant |= 0b001u;
    if (pos.y > center.y) octant |= 0b010u;
    if (pos.z > center.z) octant |= 0b100u;
    return octant;
}

Vec3D Octree::centerOfOctant(const Vec3D &currentCenter, double currentLength, uint8_t octant) {
    Vec3D newCenter = currentCenter;
    newCenter.x += (octant & 0b001u ? 0.25 : -0.25) * currentLength;
    newCenter.y += (octant & 0b010u ? 0.25 : -0.25) * currentLength;
    newCenter.z += (octant & 0b100u ? 0.25 : -0.25) * currentLength;
    return newCenter;
}

Vec3D Octree::centerOfOctant(const Vec3D &currentCenter, double currentLength, const Vec3D &pos) {
    return centerOfOctant(currentCenter, currentLength, getOctant(currentCenter, pos));
}

bool Octree::isInBounds(const Vec3D &center, double length, const Vec3D &pos) {
    bool a = center.x - length <= pos.x && pos.x <= center.x + length;
    bool b = center.y - length <= pos.y && pos.y <= center.y + length;
    bool c = center.z - length <= pos.z && pos.z <= center.z + length;
    return a && b && c;
}

void Octree::debugPrint() const {
    root.debugPrint(0);
}
