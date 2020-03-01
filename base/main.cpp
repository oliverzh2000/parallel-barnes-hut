//
// Created by Oliver Zhang on 2019-12-20.
//

#include <iostream>

#include "nbody_sim.h"
#include "star.h"
#include "../integrator/integrator.h"
#include "../force_calc/force_calc.h"
#include "../utils/stopwatch.h"

int main(int argc, char *argv[]) {
    std::cout << "3D N-body simulation with Barnes-Hut, by Oliver Zhang." << std::endl;

    std::string simDir;
    int steps = 1;
    int framesPerFullPrecisionWrite = 15; // every N frames, write full precision frame
    bool writeHumanReadable = false;
    bool writeSpaceEfficient = true;

    bool verbose = false;
    bool silent = false;

    bool showHelp = false;
    bool showVersion = false;
    bool failedArgParse = false;
    std::string unrecognizedFlag;

    // TODO: Add short-form options.
    for (int i = 1; i < argc; ++i) {
        if (std::string{argv[i]} == "--sim-directory") {
            simDir = argv[i + 1];
            i++;
            continue;
        } else if (std::string{argv[i]} == "--steps") {
            steps = std::stoi(argv[i + 1]);
            i++;
            continue;
        } else if (std::string{argv[i]} == "--verbose") {
            verbose = true;
            continue;
        } else if (std::string{argv[i]} == "--silent") {
            silent = true;
            continue;
        } else if (std::string{argv[i]} == "--help") {
            showHelp = true;
            continue;
        } else if (std::string{argv[i]} == "--version") {
            showVersion = true;
            continue;
        } else if (std::string{argv[i]} == "--human-readable-write") {
            writeHumanReadable = true;
            continue;
        } else if (std::string{argv[i]} == "--no-space-efficient-write") {
            writeSpaceEfficient = false;
            framesPerFullPrecisionWrite = 1;
            continue;
        } else {
            failedArgParse = true;
            unrecognizedFlag = std::string{argv[i]};
            break;
        }
    }
    if (showHelp) {
        std::cout << "Help menu not implemented." << std::endl;
        return 0;
    }
    if (showVersion) {
        std::cout << "Version=1.0.0" << std::endl;
        return 0;
    }
    if (steps < 1) {
        std::cout << "Fatal error: must run for at least one iteration step (use --steps to specify)" << std::endl;
        failedArgParse = true;
    }
    // Required switches.
    if (simDir.empty()) {
        std::cout << "Fatal error: no simulation directory (use --sim-directory to specify)" << std::endl;
        failedArgParse = true;
    }
    if (failedArgParse) {
        if (!unrecognizedFlag.empty()) {
            std::cout << "Unrecognized command line option: " << unrecognizedFlag << std::endl;
        }
        std::cout << "Try --help for more information" << std::endl;
        return 1;
    }

    Stopwatch::setDoOutput(verbose);

    std::unique_ptr<NBodySim> sim;
    try {
        auto s1 = Stopwatch::createAndStart("init");
        sim = std::make_unique<NBodySim>(NBodySim::readFromFile(simDir));
        s1.stopAndOutput();
    } catch (const std::runtime_error &e) {
        std::cout << "Fatal error: parsing simulation init file" << std::endl;
        std::cout << e.what() << std::endl;
        return 1;
    }

    for (int i = 0; i < steps; ++i) {
        sim->advanceSingleStep();
        bool writeFullPrecision = i % framesPerFullPrecisionWrite == 0 || i == steps - 1;
        auto s2 = Stopwatch::createAndStart("write to file");
        sim->writeToFile(simDir, writeHumanReadable, writeFullPrecision, writeSpaceEfficient);
        s2.stopAndOutput();
        if (!silent) {
            std::cout << "Progress: " << std::to_string(i + 1) << "/" + std::to_string(steps) << " (" << (i + 1) / double(steps) * 100 << "%)" << std::endl;
        }
    }

    if (!silent) {
        std::cout << "\nDone." << std::endl;
    }
}
