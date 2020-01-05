//
// Created by Oliver Zhang on 2020-01-04.
//

#include "integrator_leapfrog.h"
#include "../force_calc/force_calc.h"
#include "../model.h"

IntegratorLeapfrog::IntegratorLeapfrog(double timestep) : Integrator(timestep) {}

void IntegratorLeapfrog::advanceSingleStep(const ForceCalc &forceCalc, Model &model) {
    for (int i = 0; i < model.size(); ++i) {
        model.pos(i) += model.vel(i) * timestep  * 0.5;
    }
    forceCalc.updateNetAccel(model);
    for (int i = 0; i < model.size(); ++i) {
        model.vel(i) += model.acc(i) * timestep;
    }
    for (int i = 0; i < model.size(); ++i) {
        model.pos(i) += model.vel(i) * timestep  * 0.5;
    }
}
