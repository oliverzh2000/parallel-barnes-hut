//
// Created by Oliver Zhang on 2019-12-20.
//

#ifndef BH_SIM_INTEGRATOR_H
#define BH_SIM_INTEGRATOR_H

#include <ostream>

class Model;
class ForceCalc;

/**
 * Abstract base class for an integration scheme that uses acceleration (computed by a ForceCalc)
 * to update velocity to update position for a Model of Stars.
 */
class Integrator {
protected:
    double timestep;
    int timestepCount = 0;

public:
    explicit Integrator(double timestep);

    Integrator(double timestep, int timestepCount);

    /// Update the positions and velocities of each of the stars in the given Model by one timestep.
    /// Assumes that ForceCalc::computeNetAccel has already been called on the Model for the current timestep.
    virtual void advanceSingleStep(const ForceCalc &forceCalc, Model &model) = 0;

    /// Return the elapsed time since the start of the simulation (when time = 0).
    /// Each iteration of advanceSingleStep increases elapsed time by one timestep.
    double elapsedTime();

    /// Return the number of timesteps taken since the beginning of the simulation (when timestepCount = 0).
    int getTimestepCount();

    void setTimestepCount(int count);
};


#endif //BH_SIM_INTEGRATOR_H
