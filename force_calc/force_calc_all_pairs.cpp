//
// Created by Oliver Zhang on 2019-12-23.
//

#include "force_calc_all_pairs.h"
#include "../vec3d.h"
#include "../model.h"

ForceCalcAllPairs::ForceCalcAllPairs(double gravConst, double softening) : ForceCalc(gravConst, softening) {}

void ForceCalcAllPairs::updateNetAccel(Model &model) {
    for (int i = 0; i < model.size(); ++i) {
        model.acc(i) = {0, 0, 0};
    }
    for (int i = 0; i < model.size(); ++i) {
        for (int j = 0; j < model.size(); ++j) {
            if (i != j) {
//                double r = model.pos(i).distanceTo(model.pos(j));
//                model.acc(i) += (model.pos(j) - model.pos(i)) *
//                                (gravConst * model.mass(j) / (r * r + softeningSquared) * r);
                model.acc(i) += computeAcc(model.pos(i), model.pos(j), model.mass(j));
            }
        }
    }
}

void ForceCalcAllPairs::afterAdvanceSingleStep() {}
