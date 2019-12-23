//
// Created by Oliver Zhang on 2019-12-20.
//

#include <iostream>
#include "NBodySim.h"
#include "integrator/integrator.h"
#include "force_calc/force_calc.h"

int main() {
    std::cout << "N-body simulation using Barnes-Hut, by Oliver Zhang." << std::endl;
    double gravConst = 1.0;
    double timestep = 1.0;

    auto *sim = new NBodySim{new Integrator{timestep}, new ForceCalc{gravConst}};

    std::cout << "hello" << std::endl;
}
