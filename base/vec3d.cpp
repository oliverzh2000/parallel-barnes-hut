//
// Created by Oliver Zhang on 2019-12-20.
//

#include "vec3d.h"

#include <cmath>

double Vec3D::distanceTo(const Vec3D &other) const {
    return std::sqrt((this->x - other.x) * (this->x - other.x) +
                     (this->y - other.y) * (this->y - other.y) +
                     (this->z - other.z) * (this->z - other.z));
}

double Vec3D::length() const {
    return distanceTo({0, 0, 0});
}

Vec3D &Vec3D::operator+=(const Vec3D &other) {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    return *this;
}

Vec3D Vec3D::operator+(const Vec3D &other) const {
    return Vec3D{*this} += other;
}

Vec3D &Vec3D::operator-=(const Vec3D &other) {
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    return *this;
}

Vec3D Vec3D::operator-(const Vec3D &other) const {
    return Vec3D{*this} -= other;
}

Vec3D &Vec3D::operator*=(double k) {
    this->x *= k;
    this->y *= k;
    this->z *= k;
    return *this;
}

Vec3D Vec3D::operator*(double k) const {
    return Vec3D{*this} *= k;
}
std::ostream &operator<<(std::ostream &out, const Vec3D &vec3D) {
    out << "(" << vec3D.x << ", " << vec3D.y << ", " << vec3D.z << ")";
    return out;
}
