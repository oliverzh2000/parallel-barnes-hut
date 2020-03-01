//
// Created by Oliver Zhang on 2019-12-21.
//

#ifndef BH_SIM_STAR_H
#define BH_SIM_STAR_H

#include "vec3d.h"

struct Star {
    Vec3D pos;
    Vec3D vel;
    Vec3D acc;
    double mass;

    Star(Vec3D pos, Vec3D vel, double mass);
};

#endif // BH_SIM_STAR_H
