//
// Created by Oliver Zhang on 2019-12-20.
//

#include <iostream>
#include <fstream>

#include "nbody_sim.h"
#include "star.h"
#include "../integrator/integrator.h"
#include "../force_calc/force_calc.h"
#include "../integrator/integrator_euler.h"
#include "../force_calc/force_calc_all_pairs.h"
#include "../force_calc/force_calc_barnes_hut.h"
#include "../performance_stats/stopwatch.h"

int main(int argc, char *argv[]) {
    std::cout << "3D N-body simulation with Barnes-Hut, by Oliver Zhang." << std::endl;
    
    std::string simDir = "sim_data/sim5/";
    int n = 1;
    int nPerWrite = 1;
    bool writeToFile = false;

    auto s1 = Stopwatch::createAndStart("init");
    std::ifstream inFile{simDir + "in.txt"};
    NBodySim sim = NBodySim::readFromFile(inFile);
    s1.stopAndOutput();

    for (int i = 0; i < n; ++i) {
        sim.advanceSingleStep();
        if (i % nPerWrite == 0) {
            if (writeToFile) {
                std::ofstream outFile{simDir + "out-" + std::to_string(i) + ".txt"};
                sim.writeToFile(outFile);
            }
            std::cout << "% Progress: " << (i + 1) / double(n) * 100 << std::endl;
        }
    }

    std::cout << "done." << std::endl;
}
