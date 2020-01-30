//
// Created by Oliver Zhang on 2019-12-20.
//

#ifndef BH_SIM_VEC3D_H
#define BH_SIM_VEC3D_H

#include <ostream>

struct Vec3D {
    double x = 0;
    double y = 0;
    double z = 0;

public:
    /// Return the (euclidean) distance from this to the given Vec3D.
    double distanceTo(const Vec3D &other) const;

    Vec3D &operator+=(const Vec3D &other);

    Vec3D operator+(const Vec3D &other) const;

    Vec3D &operator-=(const Vec3D &other);

    Vec3D operator-(const Vec3D &other) const;

    Vec3D &operator*=(double k);

    Vec3D operator*(double k) const;

    friend std::ostream &operator<<(std::ostream &out, const Vec3D &vec3D);
};


#endif //BH_SIM_VEC3D_H
