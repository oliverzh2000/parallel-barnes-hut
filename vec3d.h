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
    /// Return the (euclidean) distance from this to the given Vec3D.
    double distanceTo(const Vec3D &other) const;

    Vec3D &operator+=(const Vec3D &other);

    const Vec3D operator+(const Vec3D &other) const;

    Vec3D &operator-=(const Vec3D &other);

    const Vec3D operator-(const Vec3D &other) const;

    Vec3D &operator*=(double k);

    const Vec3D operator*(double k) const;
};


#endif //BH_SIM_VEC3D_H
