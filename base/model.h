//
// Created by Oliver Zhang on 2019-12-20.
//

#ifndef BH_SIM_MODEL_H
#define BH_SIM_MODEL_H


#include <vector>
#include <iostream>
#include "vec3d.h"

class Star;

/**
 * A simple container for the stars inside a simulation.
 */
class Model final {
    std::vector<Star> stars;

public:
    /// Add a new star into this model.
    void addStar(Star star);

    Vec3D &pos(int i);

    Vec3D &vel(int i);

    Vec3D &acc(int i);

    double &mass(int i);

    /// Get a read-only version of the Stars represented by this Model.
    const std::vector<Star> &getStars() const;

    void appendFrom(const Model &other);

    /// Get the number of Stars this Model currently holds
    int size() const;

    void serialize(std::ostream &out);
    void deSerialize(std::istream &in);
};


#endif //BH_SIM_MODEL_H
