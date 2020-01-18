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
#include "performance_stats/stopwatch.h"

int main() {
    std::cout << "N-body Barnes-Hut simulation, by Oliver Zhang." << std::endl;
    Stopwatch::setOutput(true);

    std::string simDir = "sim_data/sim5/";
    std::ifstream inFile{simDir + "in.txt"};
    NBodySim sim = NBodySim::readFromFile(inFile);

    Stopwatch s1 = Stopwatch::createAndStart("create galaxy");
    sim.addXYPlaneSpiralGalaxy(500000, {}, {}, 100, 1, 0.1, 0);
    s1.stopAndOutput();

    int n = 1;
    int nPerWrite = 1;
    bool writeToFile = false;

    for (int i = 0; i < n; ++i) {
        sim.advanceSingleStep();
        if (i % nPerWrite == 0) {
            if (writeToFile) {
                std::ofstream outFile{simDir + "out-" + std::to_string(i) + ".txt"};
                sim.writeToFile(outFile);
            }
            std::cout << "% progress: " << (i + 1) / double(n) * 100 << std::endl;
        }
    }

    std::cout << "done." << std::endl;
}
