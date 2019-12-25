//
// Created by Oliver Zhang on 2019-12-20.
//

#include <iostream>
#include "NBodySim.h"
#include "star.h"
#include "integrator/integrator.h"
#include "force_calc/force_calc.h"
#include "integrator/integrator_euler.h"
#include "force_calc/brute_force_calc.h"

int main() {
    std::cout << "N-body simulation using Barnes-Hut, by Oliver Zhang." << std::endl;
    double gravConst = 1.0;
    double softening = 0.01;
    double timestep = 1.0;

    auto *sim = new NBodySim{new IntegratorEuler{timestep}, new BruteForceCalc{gravConst, softening}};

    sim->addStar({{0, 0, 0}, {0, 0, 0}, 1});
    sim->addStar({{1, 0, 0}, {0, 0, 0}, 1});

    sim->advanceSingleStep();

    std::cout << "hello" << std::endl;
}
