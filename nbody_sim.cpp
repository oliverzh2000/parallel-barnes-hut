//
// Created by Oliver Zhang on 2019-12-20.
//

#include "nbody_sim.h"
#include "integrator/integrator.h"
#include "force_calc/force_calc.h"
#include "star.h"
#include "force_calc/force_calc_all_pairs.h"
#include "integrator/integrator_euler.h"
#include "force_calc/force_calc_barnes_hut.h"
#include <fstream>
#include <limits>

NBodySim::NBodySim(Integrator *integrator, ForceCalc *forceCalc)
        : integrator{std::unique_ptr<Integrator>(integrator)}, forceCalc{std::unique_ptr<ForceCalc>(forceCalc)} {}

void NBodySim::advanceSingleStep() {
    forceCalc->updateNetAccel(model);
    integrator->advanceSingleStep(model);
}

void
NBodySim::addSpiralGalaxy(int n, Vec3D centerPos, Vec3D centerVel, double radius, double radiusStdDev, double avgMass,
                          double massStdDev) {

}

void NBodySim::addStar(Star star) {
    model.addStar(star);
}

double NBodySim::readParamFromName(std::istream &in, std::string expectedName) {
    std::string name;
    in >> name;
    if (name != expectedName) {
        throw std::runtime_error{expectedName + " missing or incomplete"};
    }
    double value;
    in >> value;
    return value;
}

NBodySim NBodySim::readFromFile(std::istream &in) {
    // TODO: Replace this hardcoded parsing with json or at least make a dedicated parser and serializer.
    Integrator *integrator;
    std::string integratorName;
    in >> integratorName;
    if (integratorName == "IntegratorEuler") {
        integrator = new IntegratorEuler(readParamFromName(in, "timestep"));
    } else {
        throw std::runtime_error{"invalid integrator name"};
    }

    ForceCalc *forceCalc;
    std::string forceCalcName;
    in >> forceCalcName;
    if (forceCalcName == "ForceCalcAllPairs") {
        forceCalc = new ForceCalcAllPairs{readParamFromName(in, "gravConst"),
                                          readParamFromName(in, "softening")};
    } else if (forceCalcName == "ForceCalcBarnesHut") {
        forceCalc = new ForceCalcBarnesHut{readParamFromName(in, "gravConst"),
                                           readParamFromName(in, "softening"),
                                           readParamFromName(in, "theta")};
    } else {
        throw std::runtime_error{"invalid forceCalc name"};
    }

    NBodySim sim = NBodySim{integrator, forceCalc};
    int n;
    in >> n;
    for (int i = 0; i < n; ++i) {
        double posX, posY, posZ, velX, velY, velZ, mass;
        in >> posX >> posY >> posZ >> velX >> velY >> velZ >> mass;
        sim.addStar({{posX, posY, posZ}, {velX, velY, velZ}, mass});
    }
    return sim;
}

void NBodySim::writeToFile(std::ostream &out) {
    unsigned long long int n = model.getStars().size();
    out << n << std::endl;
    for (const Star &star: model.getStars()) {
        out.precision(std::numeric_limits<double>::max_digits10 + 2);
        out << star.pos.x << " "<< star.pos.y << " " << star.pos.z << " " << star.vel.x << " " << star.vel.y << " " << star.vel.z << " " << star.mass << std::endl;
    }
}
