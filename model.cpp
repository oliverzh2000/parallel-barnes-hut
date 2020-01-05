//
// Created by Oliver Zhang on 2019-12-20.
//

#include "model.h"
#include "star.h"

void Model::addStar(Star star) {
    // TODO: move semantics
    stars.emplace_back(star);
}

size_t Model::size() {
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
