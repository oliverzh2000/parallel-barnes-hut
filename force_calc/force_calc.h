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
    virtual void updateNetAccel(Model &model) const = 0;

    /**
     * Returns the gravitational field due to an object at the given position.
     * @param pos Position of a 'test particle' to evaluate grav field at
     * @param sourcePos Position of the source object
     * @param mass Mass of the source object
     * @return the vector value of the grav. field due to source at the given position
     */
    Vec3D gravFieldDueToSingleObject(const Vec3D &sourcePos, double mass, const Vec3D &pos) const;
};


#endif //BH_SIM_FORCECALCULATOR_H
