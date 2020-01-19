//
// Created by Oliver Zhang on 2019-12-20.
//

#include "model.h"
#include "star.h"

void Model::addStar(Star star) {
    stars.emplace_back(star);
}

int Model::size() const {
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

void Model::serialize(std::ostream &out) {
    out << size();
    for (const Star &star: stars) {
        double starData[7]{star.pos.x,
                           star.pos.y,
                           star.pos.z,
                           star.vel.x,
                           star.vel.y,
                           star.vel.z,
                           star.mass};
        out.write(reinterpret_cast<char *>(starData), sizeof(starData));
    }
}

void Model::deSerialize(std::istream &in) {
    int n;
    in >> n;
    for (int i = 0; i < n; ++i) {
        char rawData[7 * 8];
        in.read(rawData, sizeof(rawData));
        auto *starData = reinterpret_cast<double *>(rawData);
        addStar({{starData[0],
                  starData[1],
                  starData[2]},
                 {starData[3],
                  starData[4],
                  starData[5]},
                 starData[6]});
    }
}
