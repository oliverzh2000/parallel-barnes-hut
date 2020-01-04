//
// Created by Oliver Zhang on 2019-12-20.
//

#include "force_calc.h"

ForceCalc::ForceCalc(double gravConst, double softening)
        : gravConst{gravConst}, softeningSquared{softening * softening} {}

Vec3D ForceCalc::computeAcc(const Vec3D &pos, const Vec3D &otherPos, double mass) {
    double r = otherPos.distanceTo(pos);
    return (otherPos - pos) * (gravConst * mass / (r * r + softeningSquared) * r);
}
