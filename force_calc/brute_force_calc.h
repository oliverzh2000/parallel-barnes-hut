//
// Created by Oliver Zhang on 2019-12-23.
//

#ifndef BH_SIM_BRUTEFORCECALC_H
#define BH_SIM_BRUTEFORCECALC_H

#include "force_calc.h"

class BruteForceCalc : public ForceCalc {
public:
    explicit BruteForceCalc(double gravConst);

    /// Compute the acceleration of each star in the given Model and write it in each Star's corresponding acc field.
    /// Runs in quadratic time.
    void updateNetAccel(Model &model) override;

    void afterAdvanceSingleStep() override;
};


#endif //BH_SIM_BRUTEFORCECALC_H
