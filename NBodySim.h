//
// Created by Oliver Zhang on 2019-12-20.
//

#ifndef BH_SIM_SIMULATION_H
#define BH_SIM_SIMULATION_H

#include <vector>
#include <iostream>
#include <memory>

#include "model.h"

class Vec3D;

class Integrator;

class ForceCalc;

class Star;

class NBodySim {
    Model model;
    std::unique_ptr<Integrator> integrator;
    std::unique_ptr<ForceCalc> forceCalc;

public:
    NBodySim(Integrator *integrator, ForceCalc *forceCalc);

    void advanceSingleStep();

    void addSpiralGalaxy(int n, Vec3D centerPos, Vec3D centerVel, double radius, double radiusStdDev, double avgMass,
                         double massStdDev);

    void addStar(Star star);

    const std::vector<Star> &getStars() const;
};


#endif //BH_SIM_SIMULATION_H
