//
// Created by Oliver Zhang on 2019-12-20.
//

#include "nbody_sim.h"
#include "integrator/integrator.h"
#include "force_calc/force_calc.h"
#include "star.h"
#include "force_calc/force_calc_exact.h"
#include "integrator/integrator_euler.h"
#include <fstream>
#include <limits>

NBodySim::NBodySim(Integrator *integrator, ForceCalc *forceCalc)
        : integrator{std::unique_ptr<Integrator>(integrator)}, forceCalc{std::unique_ptr<ForceCalc>(forceCalc)} {}

void NBodySim::advanceSingleStep() {
    forceCalc->updateNetAccel(model);
    integrator->advanceSingleStep(model);
    forceCalc->afterAdvanceSingleStep();
}

void
NBodySim::addSpiralGalaxy(int n, Vec3D centerPos, Vec3D centerVel, double radius, double radiusStdDev, double avgMass,
                          double massStdDev) {

}

void NBodySim::addStar(Star star) {
    model.addStar(star);
}

const std::vector<Star> &NBodySim::getStars() const {
    return model.getStars();
}

NBodySim NBodySim::readFromFile(std::istream &in) {
    // TODO: implement deserialization of the integrator and force calculator, instead of hardcoding it here.
    double gravConst = 1.0;
    double softening = 0.01;
    double timestep = 0.01;
    NBodySim sim = NBodySim{new IntegratorEuler{timestep}, new ForceCalcExact{gravConst, softening}};
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
    // TODO: implement serialization of the integrator and force calculator.
    unsigned long long int n = getStars().size();
    out << n << std::endl;
    for (const Star &star: getStars()) {
        out.precision(std::numeric_limits<double>::max_digits10 + 2);
        out << star.pos.x << " "<< star.pos.y << " " << star.pos.z << " " << star.vel.x << " " << star.vel.y << " " << star.vel.z << " " << star.mass << std::endl;
    }
}
