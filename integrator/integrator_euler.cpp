//
// Created by Oliver Zhang on 2019-12-23.
//

#include "integrator_euler.h"
#include "../model.h"
#include "../force_calc/force_calc.h"

IntegratorEuler::IntegratorEuler(double timestep) : Integrator(timestep) {}

void IntegratorEuler::advanceSingleStep(const ForceCalc &forceCalc, Model &model) {
    forceCalc.updateNetAccel(model);
    for (int i = 0; i < model.size(); ++i) {
        model.vel(i) += model.acc(i) * timestep;
    }
    for (int i = 0; i < model.size(); ++i) {
        model.pos(i) += model.vel(i) * timestep;
    }
    timestepCount++;
}
