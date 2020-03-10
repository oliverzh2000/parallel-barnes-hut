//
// Created by Oliver Zhang on 2019-12-20.
//

#ifndef BH_SIM_SIMULATION_H
#define BH_SIM_SIMULATION_H

#include "base/model.h"
#include "model.h"

#include <iostream>
#include <memory>
#include <random>
#include <vector>

class Vec3D;
class Integrator;
class ForceCalc;
class Star;

class NBodySim final {
    Model model;
    std::unique_ptr<Integrator> integrator;
    std::unique_ptr<ForceCalc> forceCalc;

  public:
    NBodySim(Integrator *integrator, ForceCalc *forceCalc);

    static NBodySim readFromFile(const std::string &simDir);

    void writeToFile(const std::string &simDir, bool writeHumanReadable, bool writeFullPrecision, bool writeSpaceEfficient);

    void advanceSingleStep();

    /**
     * Add n total stars/black holes into this model uniformly distributed in a sphere.
     * @param n total number of stars + black holes
     * @param centerPos (geometric) center position
     * @param centerVel center of mass velocity
     * @param radius radius of the sphere
     * @param starSpeedStdDev velocity standard deviation
     * @param avgStarMass average star mass
     * @param starMassStdDev star mass standard deviation
     * @param nBlackHoles number of black holes
     * @param avgBlackHoleMass average black hole mass
     * @param blackHoleMassStdDev black hole mass standard deviation
     * @param seed RNG seed
     */
    void addUniformSphericalBlob(int n, Vec3D centerPos, Vec3D centerVel, double radius, double avgStarSpeed, double starSpeedStdDev,
                                 double starMassStdDev, int nBlackHoles, double avgBlackHoleMass, double blackHoleMassStdDev, int seed,
                                 double avgStarMass);

    void addStar(Star star);

  private:
    template <typename T>
    static T readParamByName(std::istream &in, const std::string &expectedName);

    // TODO: merge the separate Vec3D method if possible.
    static Vec3D readVec3DParamByName(std::istream &in, const std::string &expectedName);

    template <typename T>
    static void writeParamWithName(std::ostream &out, const std::string &name, T value, int indentLevel);

    static void verifyParamName(std::istream &in, const std::string &expectedName);

    // 1. Create uniformly distributed point on the unit sphere. This actually done with normal distributions (remember: you use 2 normal
    //   distributions, one in x and one in y to create a circular distribution)
    // 2. Scale that point to be on the unit circle.
    // 3. Multiply point by random normally distributed magnitude.
    Vec3D generateSphericalPoint(std::default_random_engine unifRNG, double avgMagnitude, double stdDev);
};

#endif // BH_SIM_SIMULATION_H
