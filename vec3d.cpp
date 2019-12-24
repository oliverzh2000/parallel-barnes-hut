//
// Created by Oliver Zhang on 2019-12-20.
//

#include <cmath>
#include "vec3d.h"

double Vec3D::distanceTo(const Vec3D &other) const {
    return std::sqrt((this->x - other.x) * (this->x - other.x) +
                     (this->y - other.y) * (this->y - other.y) +
                     (this->z - other.z) * (this->z - other.z));
}

Vec3D &Vec3D::operator+=(const Vec3D &other) {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    return *this;
}


const Vec3D Vec3D::operator+(const Vec3D &other) const {
    return Vec3D{*this} += other;
}

Vec3D &Vec3D::operator-=(const Vec3D &other) {
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    return *this;
}

const Vec3D Vec3D::operator-(const Vec3D &other) const {
    return Vec3D{*this} -= other;
}

Vec3D &Vec3D::operator*=(double k) {
    this->x *= k;
    this->y *= k;
    this->z *= k;
    return *this;
}

Vec3D &Vec3D::operator*(double k) const {
    return Vec3D{*this} *= k;
}


