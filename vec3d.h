//
// Created by Oliver Zhang on 2019-12-20.
//

#ifndef BH_SIM_VEC3D_H
#define BH_SIM_VEC3D_H


struct Vec3D {
    double x;
    double y;
    double z;

public:
    double distanceSquared(const Vec3D &other);
};


#endif //BH_SIM_VEC3D_H
