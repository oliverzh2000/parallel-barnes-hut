//
// Created by Oliver Zhang on 2020-01-04.
//

#ifndef BH_SIM_INTEGRATORLEAPFROG_H
#define BH_SIM_INTEGRATORLEAPFROG_H

#include "integrator.h"

class IntegratorLeapfrog : public Integrator {
  public:
    explicit IntegratorLeapfrog(double timestep);

    IntegratorLeapfrog(double timestep, int timestepCount);

    void advanceSingleStep(const ForceCalc &forceCalc, Model &model) override;
};

#endif // BH_SIM_INTEGRATORLEAPFROG_H
