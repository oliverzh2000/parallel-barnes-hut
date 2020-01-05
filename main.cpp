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
#include "force_calc/force_calc_all_pairs.h"
#include "force_calc/force_calc_barnes_hut.h"

int main() {
    std::cout << "N-body Barnes-Hut simulation, by Oliver Zhang." << std::endl;

    std::string simDir = "sim_data/sim6/";
    std::ifstream inFile{simDir + "in.txt"};
    NBodySim sim = NBodySim::readFromFile(inFile);
    int n = 500000000;
    for (int i = 0; i < n; ++i) {
        sim.advanceSingleStep();
//        if (i % 10000 == 0) {
//            std::ofstream outFile{simDir + "out-" + std::to_string(i)  + ".txt"};
//            sim.writeToFile(outFile);
//            std::cout << i / double(n) * 100 << std::endl;
//        }
    }

    std::cout << "hello" << std::endl;
}
