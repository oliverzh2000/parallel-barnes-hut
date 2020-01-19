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
#include "../utils/stopwatch.h"

int main(int argc, char *argv[]) {
    std::cout << "3D N-body simulation with Barnes-Hut, by Oliver Zhang." << std::endl;

    std::string simDir;
    int n = 10;
    int nPerWrite = 1;
    bool verbose = false;
    bool silent = false;

    bool showHelp = false;
    bool failedArgParse = false;
    std::string invalidFlag;
    for (int i = 1; i < argc; ++i) {
        if (std::string{argv[i]} == "--sim-directory") {
            simDir = argv[i + 1];
            i++;
        } else if (std::string{argv[i]} == "--steps") {
            n = std::stoi(argv[i + 1]);
            i++;
        } else if (std::string{argv[i]} == "--verbose") {
            verbose = true;
        } else if (std::string{argv[i]} == "--slient") {
            silent = true;
        } else if (std::string{argv[i]} == "--help") {
            showHelp = true;
        } else if (std::string{argv[i]} == "--version") {
            std::cout << "version=1.0.0" << std::endl;
            return 0;
        } else {
            failedArgParse = true;
            invalidFlag = std::string{argv[i]};
            break;
        }
    }

    // Required switches.
    if (simDir.empty()) {
        std::cout << "fatal error: no simulation directory" << std::endl;
        failedArgParse = true;
    }

    if (showHelp) {
        std::cout << "Help menu not implemented." << std::endl;
    }
    if (failedArgParse) {
        if (!invalidFlag.empty()) {
            std::cout << "N-body: unrecognized command line option: " << invalidFlag << std::endl;
        }
        std::cout << "try --help for more information" << std::endl;
        return 1;
    }

    Stopwatch::setDoOutput(verbose);

    auto s1 = Stopwatch::createAndStart("init");
    NBodySim sim = NBodySim::readFromFile(simDir);
    s1.stopAndOutput();

    for (int i = 0; i < n; ++i) {
        sim.advanceSingleStep();
        if (i % nPerWrite == 0) {
            sim.writeToFile(simDir, false);
            if (!silent) {
                std::cout << "Progress: " << std::to_string(i + 1) << "/" + std::to_string(n) << " (" << (i + 1) / double(n) * 100 << "%)" << std::endl;
            }
        }
    }

    std::cout << "done." << std::endl;
}
