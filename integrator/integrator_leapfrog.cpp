//
// Created by Oliver Zhang on 2020-01-04.
//

#include "integrator_leapfrog.h"

#include "base/model.h"
#include "force_calc/force_calc.h"
#include "utils/stopwatch.h"

IntegratorLeapfrog::IntegratorLeapfrog(double timestep) : Integrator(timestep) {}

IntegratorLeapfrog::IntegratorLeapfrog(double timestep, int timestepCount) : Integrator(timestep, timestepCount) {}

void IntegratorLeapfrog::advanceSingleStep(const ForceCalc &forceCalc, Model &model) {
    auto s1 = Stopwatch::createAndStart("pos += vel *dt / 2");
    for (int i = 0; i < model.size(); ++i) {
        model.pos(i) += model.vel(i) * timestep * 0.5;
    }
    s1.stopAndOutput();

    auto s2 = Stopwatch::createAndStart("update net accel");
    forceCalc.updateNetAccel(model);
    s2.stopAndOutput();

    auto s3 = Stopwatch::createAndStart("vel += acc * dt");
    for (int i = 0; i < model.size(); ++i) {
        model.vel(i) += model.acc(i) * timestep;
    }
    s3.stopAndOutput();

    auto s4 = Stopwatch::createAndStart("pos += vel *dt / 2");
    for (int i = 0; i < model.size(); ++i) {
        model.pos(i) += model.vel(i) * timestep * 0.5;
    }
    s4.stopAndOutput();
    timestepCount++;
}
