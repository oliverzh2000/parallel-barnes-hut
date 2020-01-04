//
// Created by Oliver Zhang on 2019-12-20.
//

#ifndef BH_SIM_FORCECALCULATOR_H
#define BH_SIM_FORCECALCULATOR_H

#include "../vec3d.h"
#include "../star.h"

class Model;

/**
 * Abstract base class for force/acceleration computation on a Model of Stars.
 */
class ForceCalc {
protected:
    double gravConst;
    double softeningSquared;

public:
    explicit ForceCalc(double gravConst, double softening);

    /// Compute the acceleration of each star in the given Model and write it in each Star's corresponding acc field.
    virtual void updateNetAccel(Model &model) = 0;

    /// Code to be executed after one timestep has elapsed.
    virtual void afterAdvanceSingleStep() = 0;

    Vec3D computeAcc(const Vec3D &pos, const Vec3D &otherPos, double mass);
};


#endif //BH_SIM_FORCECALCULATOR_H
