//
// Created by Oliver Zhang on 2019-12-20.
//

#include <iostream>
#include <fstream>
#include "nbody_sim.h"
#include "star.h"
#include "integrator/integrator.h"
#include "force_calc/force_calc.h"
#include "integrator/integrator_euler.h"
#include "force_calc/force_calc_exact.h"

int main() {
    std::cout << "N-body Barnes-Hut simulation, by Oliver Zhang." << std::endl;

    std::ifstream inFile{"sim_data/sim3/in.txt"};
    NBodySim sim = NBodySim::readFromFile(inFile);

    for (int i = 0; i < 500; ++i) {
        sim.advanceSingleStep();
        std::ofstream outFile{"sim_data/sim3/out-" + std::to_string(i)  + ".txt"};
        sim.writeToFile(outFile);
    }

    std::cout << "hello" << std::endl;
}
