//
// Created by Oliver Zhang on 2019-12-20.
//

#include "model.h"
#include "star.h"

void Model::addStar(Star star) {
    // TODO: move semantics
    stars.emplace_back(star);
}

void Model::setPos(int i, Vec3D pos) {
    stars.at(i).pos = pos;
}

void Model::setVel(int i, Vec3D vel) {
    stars.at(i).vel = vel;
}

void Model::setAcc(int i, Vec3D acc) {
    stars.at(i).acc = acc;
}

Vec3D Model::getPos(int i) {
    return stars.at(i).pos;
}

Vec3D Model::getVel(int i) {
    return stars.at(i).vel;
}

Vec3D Model::getAcc(int i) {
    return stars.at(i).acc;
}

const std::vector<Star> &Model::getStars() const {
    return stars;
}
