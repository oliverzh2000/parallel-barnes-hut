//
// Created by Oliver Zhang on 2019-12-20.
//

#include "force_calc.h"

ForceCalc::ForceCalc(double gravConst, double softening)
        : gravConst{gravConst}, softeningSquared{softening * softening} {}
