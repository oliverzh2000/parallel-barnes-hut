//
// Created by Oliver Zhang on 2019-12-21.
//

#include "star.h"

Star::Star(Vec3D pos, Vec3D vel, double mass) : pos{pos}, vel{vel}, acc{Vec3D{0, 0, 0}}, mass{mass} {}
