//
// Created by Oliver Zhang on 2019-12-20.
//

#include "nbody_sim.h"

#include <utility>
#include <fstream>
#include <limits>
#include <random>

#include "../integrator/integrator.h"
#include "../force_calc/force_calc.h"
#include "../force_calc/force_calc_all_pairs.h"
#include "../integrator/integrator_euler.h"
#include "../force_calc/force_calc_barnes_hut.h"
#include "../integrator/integrator_leapfrog.h"
#include "../force_calc/force_calc_barnes_hut_parallel.h"

NBodySim::NBodySim(Integrator *integrator, ForceCalc *forceCalc)
        : integrator{std::unique_ptr<Integrator>(integrator)}, forceCalc{std::unique_ptr<ForceCalc>(forceCalc)} {}

void NBodySim::advanceSingleStep() {
    integrator->advanceSingleStep(*forceCalc, model);
}

void
NBodySim::addXYPlaneSpiralGalaxy(int n, Vec3D centerPos, Vec3D centerVel, double radialStdDev, double avgMass,
                                 double massStdDev, int seed) {
    std::default_random_engine uniformRng{seed};
    std::normal_distribution<double> radialDistanceGenerator{0, radialStdDev};
    std::normal_distribution<double> massGenerator{avgMass, massStdDev};

    Model newGalaxy;

    // 1) Randomly generate the positions of the stars.
    for (int i = 0; i < n - 1; ++i) {
        newGalaxy.addStar(
                // TODO: make this a disk after debugging done.
                {Vec3D{radialDistanceGenerator(uniformRng), radialDistanceGenerator(uniformRng),
                       radialDistanceGenerator(uniformRng)},
                 Vec3D{0, 0, 0},
                 massGenerator(uniformRng)});
    }

    // 2) Add the supermassive black hole in the center.
    double totalMass = 0;
    for (const Star &star: newGalaxy.getStars()) {
        totalMass += star.mass;
    }
    newGalaxy.addStar({Vec3D{0, 0, 0}, Vec3D{0, 0, 0}, totalMass});

    // TODO: 3) Compute gravitational field at position of every star and set it's
    //      velocity so that it is in uniform circular motion at time=0
    model.appendFrom(newGalaxy);
}

void NBodySim::addStar(Star star) {
    model.addStar(star);
}


NBodySim NBodySim::readFromFile(const std::string &simDir) {
    std::ifstream in{simDir + "/init.txt"};
    if (in.fail()) {
        throw std::runtime_error{"failed to open init.txt: " + simDir + "/init.txt"};
    }
    // TODO: Replace this hardcoded parsing with json or something better.
    Integrator *integrator;
    auto integratorType = readParamByName<std::string>(in, "integratorType");
    if (integratorType == "IntegratorEuler") {
        integrator = new IntegratorEuler(readParamByName<double>(in, "timestep"));
    } else if (integratorType == "IntegratorLeapfrog") {
        integrator = new IntegratorLeapfrog(readParamByName<double>(in, "timestep"));
    } else {
        throw std::runtime_error{"invalid integrator name"};
    }

    ForceCalc *forceCalc;
    auto forceCalcType = readParamByName<std::string>(in, "forceCalcType");
    if (forceCalcType == "ForceCalcAllPairs") {
        auto gravConst = readParamByName<double>(in, "gravConst");
        auto softening = readParamByName<double>(in, "softening");
        forceCalc = new ForceCalcAllPairs{gravConst, softening};
    } else if (forceCalcType == "ForceCalcBarnesHut") {
        auto gravConst = readParamByName<double>(in, "gravConst");
        auto softening = readParamByName<double>(in, "softening");
        auto theta = readParamByName<double>(in, "theta");
        forceCalc = new ForceCalcBarnesHut{gravConst, softening, theta};
    } else if (forceCalcType == "ForceCalcBarnesHutParallel") {
        auto gravConst = readParamByName<double>(in, "gravConst");
        auto softening = readParamByName<double>(in, "softening");
        auto theta = readParamByName<double>(in, "theta");
        forceCalc = new ForceCalcBarnesHutParallel{gravConst, softening, theta};
    } else {
        throw std::runtime_error{"invalid forceCalc name"};
    }

    NBodySim sim = NBodySim{integrator, forceCalc};

    auto starsInitMode = readParamByName<std::string>(in, "starsInitMode");
    if (starsInitMode == "ReadStarsInline") {
        auto n = readParamByName<int>(in, "n");
        for (int i = 0; i < n; ++i) {
            double posX, posY, posZ, velX, velY, velZ, mass;
            in >> posX >> posY >> posZ >> velX >> velY >> velZ >> mass;
            sim.addStar({{posX, posY, posZ}, {velX, velY, velZ}, mass});
        }
    } else if (starsInitMode == "ReadStarsFromLatestBinaryFrame") {
#if 0
        // TODO: Add this back in once I am able to use the <filesystem> header properly.
        int latestFameIndex = 0;
        std::filesystem::path latestFile;
        for (const auto &entry: std::filesystem::directory_iterator("frames")) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().string();
                if (entry.path().extension() == ".data" && filename.substr(0, 5) == "frame-") {
                    // At this point, filename is of the form: 'frame-0123.data',
                    // need to extract just the 0123 part.
                    int currentFrameIndex = std::stoi(filename.substr(6, filename.size() - 5));
                    if (latestFameIndex < currentFrameIndex) {
                        latestFameIndex = currentFrameIndex;
                        latestFile = entry.path();
                    }
                } else {
                    throw std::runtime_error{"frame file does not begin with 'frame-' and end with '.data' extension."};
                }
            }
        } else {
            throw std::runtime_error{"invalid starsInitMode"};
        }
        std::ifstream frameIfs{latestFile.string(), std::ios::binary};
        sim.model.deSerialize(frameIfs);
#endif
        std::ifstream frameNumberIfs{simDir + "/latest-frame-index.txt"};
        int latestBinaryFrameNumber;
        frameNumberIfs >> latestBinaryFrameNumber;
        sim.integrator->setTimestepCount(latestBinaryFrameNumber);

        std::ifstream frameIfs{simDir + "/frames/frame-" + std::to_string(latestBinaryFrameNumber) + ".data", std::ios::binary};
        sim.model.deSerialize(frameIfs);
    } else if (starsInitMode == "CreateSpiralGalaxy") {
        auto n = readParamByName<int>(in, "n");
        auto centerPos = readVec3DParamByName(in, "centerPos");
        auto centerVel = readVec3DParamByName(in, "centerVel");
        auto radialStdDev = readParamByName<double>(in, "radialStdDev");
        auto avgMass = readParamByName<double>(in, "avgMass");
        auto avgMassStdDev = readParamByName<double>(in, "avgMassStdDev");
        auto seed = readParamByName<int>(in, "seed");
        sim.addXYPlaneSpiralGalaxy(n, centerPos, centerVel, radialStdDev, avgMass, avgMassStdDev, seed);
    }
    return sim;
}

void NBodySim::writeToFile(const std::string &simDir, bool alsoWriteHumanReadable) {
    std::ofstream frameOfs{simDir + "/frames/frame-" + std::to_string(integrator->getTimestepCount()) + ".data", std::ios::binary};
    model.serialize(frameOfs);
    if (alsoWriteHumanReadable) {
        std::ofstream humanReadableFrameOfs{
                simDir + "/hr-frames/frame-" + std::to_string(integrator->getTimestepCount()) + ".data"};
        unsigned long long int n = model.getStars().size();
        humanReadableFrameOfs << n << std::endl;
        for (const Star &star: model.getStars()) {
            humanReadableFrameOfs.precision(std::numeric_limits<double>::max_digits10 + 2);
            humanReadableFrameOfs << star.pos.x << " " << star.pos.y << " " << star.pos.z << " "
                                  << star.vel.x << " " << star.vel.y << " " << star.vel.z << " "
                                  << star.mass << std::endl;
        }
    }
    std::ofstream latestFrameIndexOfs{simDir + "/latest-frame-index.txt"};
    latestFrameIndexOfs << integrator->getTimestepCount() << std::endl;
}

template<typename T>
T NBodySim::readParamByName(std::istream &in, const std::string& expectedName) {
    verifyParamName(in, expectedName);
    T value;
    in >> value;
    return value;
}

Vec3D NBodySim::readVec3DParamByName(std::istream &in, const std::string& expectedName) {
    verifyParamName(in, expectedName);
    return Vec3D{readParamByName<double>(in, "x"),
                 readParamByName<double>(in, "y"),
                 readParamByName<double>(in, "z")};
}

template<typename T>
void NBodySim::writeParamWithName(std::ostream &out, const std::string& name, T value, int indentLevel) {
    int charsPerIndent = 4;
    out << std::string(charsPerIndent * indentLevel, ' ');
    out << name << ": " << value << std::endl;
}

void NBodySim::verifyParamName(std::istream &in, const std::string& expectedName) {
    std::string nameWithColon;
    in >> nameWithColon;
    if (nameWithColon.substr(0, nameWithColon.size() - 1) != expectedName) {
        throw std::runtime_error{expectedName + " missing or incomplete."};
    }
    if (nameWithColon.back() != ':') {
        throw std::runtime_error{expectedName + " not followed by ':'."};
    }
}