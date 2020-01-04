//
// Created by Oliver Zhang on 2019-12-23.
//

#ifndef BH_SIM_BRUTEFORCECALC_H
#define BH_SIM_BRUTEFORCECALC_H

#include "force_calc.h"

class ForceCalcAllPairs : public ForceCalc {
public:
    explicit ForceCalcAllPairs(double gravConst, double softening);

    /// Runs in quadratic time.
    void updateNetAccel(Model &model) override;

};


#endif //BH_SIM_BRUTEFORCECALC_H
