//
// Created by Oliver Zhang on 2019-12-20.
//

#include "model.h"
#include "star.h"

void Model::addStar(Star star) {
    stars.emplace_back(star);
}

size_t Model::size() const {
    return stars.size();
}

Vec3D &Model::pos(int i) {
    return stars.at(i).pos;
}

Vec3D &Model::vel(int i) {
    return stars.at(i).vel;
}

Vec3D &Model::acc(int i) {
    return stars.at(i).acc;
}

const std::vector<Star> &Model::getStars() const {
    return stars;
}

double &Model::mass(int i) {
    return stars.at(i).mass;
}

void Model::appendFrom(const Model &other) {
    for (const Star &star: other.getStars()) {
        addStar(star);
    }
}
