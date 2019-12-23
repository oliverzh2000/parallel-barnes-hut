//
// Created by Oliver Zhang on 2019-12-23.
//

#ifndef BH_SIM_BRUTEFORCECALC_H
#define BH_SIM_BRUTEFORCECALC_H

#include "force_calc.h"

class BruteForceCalc : public ForceCalc {
public:
    explicit BruteForceCalc(double gravConst);

    void updateNetAccel(Model &model) override;

    void onAdvanceSingleStep() override;
};


#endif //BH_SIM_BRUTEFORCECALC_H
