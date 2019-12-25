//
// Created by Oliver Zhang on 2019-12-20.
//

#include "NBodySim.h"
#include "integrator/integrator.h"
#include "force_calc/force_calc.h"
#include "star.h"

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
