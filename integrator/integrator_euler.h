//
// Created by Oliver Zhang on 2019-12-23.
//

#ifndef BH_SIM_INTEGRATOREULER_H
#define BH_SIM_INTEGRATOREULER_H

#include "integrator.h"

class IntegratorEuler : public Integrator {
public:
    explicit IntegratorEuler(double timestep);

    void advanceSingleStep(Model &model) override;
};


#endif //BH_SIM_INTEGRATOREULER_H
