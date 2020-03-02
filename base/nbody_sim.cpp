//
// Created by Oliver Zhang on 2019-12-20.
//

#include "nbody_sim.h"

#include "force_calc/force_calc.h"
#include "force_calc/force_calc_all_pairs.h"
#include "force_calc/force_calc_barnes_hut.h"
#include "force_calc/force_calc_barnes_hut_parallel.h"
#include "integrator/integrator.h"
#include "integrator/integrator_euler.h"
#include "integrator/integrator_leapfrog.h"

#include <fstream>
#include <limits>
#include <random>
#include <utility>

NBodySim::NBodySim(Integrator *integrator, ForceCalc *forceCalc) :
    integrator{std::unique_ptr<Integrator>(integrator)}, forceCalc{std::unique_ptr<ForceCalc>(forceCalc)} {}

void NBodySim::advanceSingleStep() {
    integrator->advanceSingleStep(*forceCalc, model);
}

void NBodySim::addUniformSphericalBlob(int n, Vec3D centerPos, Vec3D centerVel, double radialStdDev, double radiusCutoff, double velStdDev,
                                       double avgMass, double massStdDev, int seed) {
    std::default_random_engine unifRNG{seed};
    std::normal_distribution<double> radialDistRNG{0, radialStdDev};
    std::normal_distribution<double> velRNG{0, velStdDev};
    std::normal_distribution<double> massRNG{avgMass, massStdDev};

    Model newGalaxy;

    // 1) Randomly generate the positions of the stars. Make sure they all are inside the radius cutoff.
    for (int i = 0; i < n - 1; ++i) {
        while (true) {
            Vec3D candidateCenterPos = Vec3D{radialDistRNG(unifRNG), radialDistRNG(unifRNG), radialDistRNG(unifRNG)};
            if (candidateCenterPos.distanceTo({0, 0, 0}) <= radiusCutoff) {
                newGalaxy.addStar({candidateCenterPos, Vec3D{velRNG(unifRNG), velRNG(unifRNG), velRNG(unifRNG)}, massRNG(unifRNG)});
                break;
            }
        }
    }
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
    } else if (starsInitMode == "ReadStarsFromLastestShortFrame") {
        std::ifstream frameNumberIfs{simDir + "/latest-frame-index.txt"};
        int latestBinaryFrameNumber;
        frameNumberIfs >> latestBinaryFrameNumber;
        sim.integrator->setTimestepCount(latestBinaryFrameNumber);

        std::ifstream frameIfs{simDir + "/short-frames/frame-" + std::to_string(latestBinaryFrameNumber) + ".data", std::ios::binary};
        sim.model.deSerializeSpaceEfficient(frameIfs);
    } else if (starsInitMode == "CreateSpiralGalaxy") {
        auto n = readParamByName<int>(in, "n");
        auto centerPos = readVec3DParamByName(in, "centerPos");
        auto centerVel = readVec3DParamByName(in, "centerVel");
        auto radialStdDev = readParamByName<double>(in, "radialStdDev");
        auto radialCutoff = readParamByName<double>(in, "radialCutoff");
        auto velStdDev = readParamByName<double>(in, "velStdDev");
        auto avgMass = readParamByName<double>(in, "avgMass");
        auto avgMassStdDev = readParamByName<double>(in, "avgMassStdDev");
        auto seed = readParamByName<int>(in, "seed");
        sim.addUniformSphericalBlob(n, centerPos, centerVel, radialStdDev, radialCutoff, velStdDev, avgMass, avgMassStdDev, seed);
    } else {
        throw std::runtime_error{"invalid stars init mode"};
    }
    return sim;
}

void NBodySim::writeToFile(const std::string &simDir, bool writeHumanReadable, bool writeFullPrecision, bool writeSpaceEfficient) {
    if (!writeHumanReadable && !writeFullPrecision && !writeSpaceEfficient) {
        throw std::runtime_error("no write performed");
    }
    std::string outFileName = "frame-" + std::to_string(integrator->getTimestepCount()) + ".data";
    if (writeFullPrecision) {
        std::ofstream frameOfs{simDir + "/frames/" + outFileName, std::ios::binary};
        model.serializeFullPrecision(frameOfs);
    }
    if (writeSpaceEfficient) {
        std::ofstream frameOfs{simDir + "/short-frames/" + outFileName, std::ios::binary};
        model.serializeSpaceEfficient(frameOfs);
    }
    if (writeHumanReadable) {
        std::ofstream frameOfs{simDir + "/hr-frames/" + outFileName};
        model.serializeHumanReadable(frameOfs);
    }
    std::ofstream latestFrameIndexOfs{simDir + "/latest-frame-index.txt"};
    latestFrameIndexOfs << integrator->getTimestepCount() << std::endl;
}

template <typename T>
T NBodySim::readParamByName(std::istream &in, const std::string &expectedName) {
    verifyParamName(in, expectedName);
    T value;
    in >> value;
    return value;
}

Vec3D NBodySim::readVec3DParamByName(std::istream &in, const std::string &expectedName) {
    verifyParamName(in, expectedName);
    return Vec3D{readParamByName<double>(in, "x"), readParamByName<double>(in, "y"), readParamByName<double>(in, "z")};
}

template <typename T>
void NBodySim::writeParamWithName(std::ostream &out, const std::string &name, T value, int indentLevel) {
    int charsPerIndent = 4;
    out << std::string(charsPerIndent * indentLevel, ' ');
    out << name << ": " << value << std::endl;
}

void NBodySim::verifyParamName(std::istream &in, const std::string &expectedName) {
    std::string nameWithColon;
    in >> nameWithColon;
    if (nameWithColon.substr(0, nameWithColon.size() - 1) != expectedName) {
        throw std::runtime_error{expectedName + " missing or incomplete."};
    }
    if (nameWithColon.back() != ':') {
        throw std::runtime_error{expectedName + " not followed by ':'."};
    }
}