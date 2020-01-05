//
// Created by Oliver Zhang on 2019-12-20.
//

#include "force_calc.h"

ForceCalc::ForceCalc(double gravConst, double softening)
        : gravConst{gravConst}, softeningSquared{softening * softening} {}

Vec3D ForceCalc::gravFieldDueToSingleObject(const Vec3D &sourcePos, double mass, const Vec3D &pos) const {
    double r = sourcePos.distanceTo(pos);
    if (r == 0) {
        return {0, 0, 0}; // For safety.
    }
    return (sourcePos - pos) * (gravConst * mass / (r * r + softeningSquared) * r);
}
