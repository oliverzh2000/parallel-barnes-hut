//
// Created by Oliver Zhang on 2019-12-20.
//

#include "model.h"

#include <algorithm>
#include <cmath>
#include <limits>
#include <tuple>

#include "star.h"
#include "vec3d.h"

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

std::tuple<Vec3D, double> Model::boundingBox() const {
    constexpr double minDouble = std::numeric_limits<double>::lowest();
    constexpr double maxDouble = std::numeric_limits<double>::max();
    Vec3D minCorner = {maxDouble, maxDouble, maxDouble};
    Vec3D maxCorner = {minDouble, minDouble, minDouble};
    for (const Star &star: getStars()) {
        if (star.pos.x < minCorner.x) minCorner.x = star.pos.x;
        if (star.pos.y < minCorner.y) minCorner.y = star.pos.y;
        if (star.pos.z < minCorner.z) minCorner.z = star.pos.z;

        if (star.pos.x > maxCorner.x) maxCorner.x = star.pos.x;
        if (star.pos.y > maxCorner.y) maxCorner.y = star.pos.y;
        if (star.pos.z > maxCorner.z) maxCorner.z = star.pos.z;
    }
    Vec3D center = minCorner * 0.5 + maxCorner * 0.5;
    double length = std::max({(center - minCorner).x, (center - minCorner).y, (center - minCorner).z}) * 2;
    return {center, length};
}

void Model::serializeFullPrecision(std::ostream &out) {
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
        char rawData[7 * sizeof(double)];
        in.read(rawData, sizeof(rawData));
        double *starData = reinterpret_cast<double *>(rawData);
        addStar({{starData[0],
                  starData[1],
                  starData[2]},
                 {starData[3],
                  starData[4],
                  starData[5]},
                 starData[6]});
    }
}

void Model::serializeSpaceEfficient(std::ostream &out) {
    out << size() << " ";

    // Determine the largest abs value possible coordinate value.
    auto[center, length] = boundingBox();
    double maxAbsX = std::abs(center.x + length / 2 * (center.x > 0 ? 1 : -1));
    double maxAbsY = std::abs(center.y + length / 2 * (center.y > 0 ? 1 : -1));
    double maxAbsZ = std::abs(center.z + length / 2 * (center.z > 0 ? 1 : -1));
    double maxAbsCoord = std::max({maxAbsX, maxAbsY, maxAbsZ});
    out << maxAbsCoord << " ";

    // Determine the largest possible mass.
    double maxMass = 0;
    for (const Star &star: getStars()) {
        if (star.mass > maxMass) {
            maxMass = star.mass;
        }
    }
    out << maxMass;

    for (const Star &star: stars) {
        Vec3D scaledPos = star.pos * (1 / maxAbsCoord) * std::numeric_limits<int16_t>::max();
        int16_t posData[3]{static_cast<int16_t>(std::round(scaledPos.x)),
                           static_cast<int16_t>(std::round(scaledPos.y)),
                           static_cast<int16_t>(std::round(scaledPos.z))};
        out.write(reinterpret_cast<char *>(posData), sizeof(posData));

        uint8_t mass[1]{static_cast<uint8_t>(std::round(star.mass / maxMass * std::numeric_limits<uint8_t>::max()))};
        out.write(reinterpret_cast<char *>(mass), sizeof(mass));
    }
}

void Model::deSerializeSpaceEfficient(std::istream &in) {
    int n;
    double maxAbsCoord;
    double maxMass;
    in >> n >> maxAbsCoord >> maxMass;
    for (int i = 0; i < n; ++i) {
        char rawPosData[3 * sizeof(int16_t)];
        in.read(rawPosData, sizeof(rawPosData));
        int16_t *unscaledPos = reinterpret_cast<int16_t *>(rawPosData);

        char rawMassData[sizeof(uint8_t)];
        in.read(rawMassData, sizeof(rawMassData));
        uint8_t *unscaledMass = reinterpret_cast<uint8_t *>(rawMassData);

        Vec3D scaledPos = Vec3D{static_cast<double>(unscaledPos[0]),
                                static_cast<double>(unscaledPos[1]),
                                static_cast<double>(unscaledPos[2])} *
                          (1 / static_cast<double>(std::numeric_limits<int16_t>::max())) * maxAbsCoord;
        double scaledMass = static_cast<double>(unscaledMass[0]) / std::numeric_limits<uint8_t>::max() * maxMass;
        addStar({scaledPos, {0, 0, 0}, scaledMass});
    }
}

void Model::serializeHumanReadable(std::ostream &out) {
    out << getStars().size() << std::endl;
    for (const Star &star: getStars()) {
        out.precision(std::numeric_limits<double>::max_digits10 + 2);
        out << star.pos.x << " " << star.pos.y << " " << star.pos.z << " "
            << star.vel.x << " " << star.vel.y << " " << star.vel.z << " "
            << star.mass << std::endl;
    }
}

void Model::deSerializeHumanReadable(std::istream &in) {
    int n;
    in >> n;
    for (int i = 0; i < n; ++i) {
        double px, py, pz, vx, vy, vz, mass;
        in >> px >> py >> pz >> vx >> vy >> vz >> mass;
        addStar({{px, py, pz}, {vx, vy, vz}, mass});
    }
}