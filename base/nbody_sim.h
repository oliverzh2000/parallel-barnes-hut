//
// Created by Oliver Zhang on 2019-12-20.
//

#ifndef BH_SIM_SIMULATION_H
#define BH_SIM_SIMULATION_H

#include "base/model.h"
#include "model.h"

#include <iostream>
#include <memory>
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

    void addUniformSphericalBlob(int n, Vec3D centerPos, Vec3D centerVel, double radialStdDev, double radiusCutoff, double velStdDev,
                                 double avgMass, double massStdDev, int seed);

    void addStar(Star star);

  private:
    template <typename T>
    static T readParamByName(std::istream &in, const std::string &expectedName);

    // TODO: merge the separate Vec3D method if possible.
    static Vec3D readVec3DParamByName(std::istream &in, const std::string &expectedName);

    template <typename T>
    static void writeParamWithName(std::ostream &out, const std::string &name, T value, int indentLevel);

    static void verifyParamName(std::istream &in, const std::string &expectedName);
};

#endif // BH_SIM_SIMULATION_H
