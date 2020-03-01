//
// Created by Oliver Zhang on 2019-12-20.
//

#include "integrator.h"

Integrator::Integrator(double timestep) : timestep{timestep} {}

Integrator::Integrator(double timestep, int timestepCount) : timestep{timestep}, timestepCount{timestepCount} {}

double Integrator::elapsedTime() {
    return timestepCount * timestep;
}

int Integrator::getTimestepCount() {
    return timestepCount;
}

void Integrator::setTimestepCount(int count) {
    timestepCount = count;
}
