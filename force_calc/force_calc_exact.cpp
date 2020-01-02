//
// Created by Oliver Zhang on 2019-12-23.
//

#include "force_calc_exact.h"
#include "../vec3d.h"
#include "../model.h"

ForceCalcExact::ForceCalcExact(double gravConst, double softening) : ForceCalc(gravConst, softening) {}

void ForceCalcExact::updateNetAccel(Model &model) {
    for (int i = 0; i < model.size(); ++i) {
        model.acc(i) = {0, 0, 0};
    }
    for (int i = 0; i < model.size(); ++i) {
        for (int j = 0; j < model.size(); ++j) {
            if (i != j) {
                double r = model.pos(i).distanceTo(model.pos(j));
                model.acc(i) += (model.pos(j) - model.pos(i)) *
                                (gravConst * model.mass(j) / (r * r + softeningSquared) * r);
            }
        }
    }
}

void ForceCalcExact::afterAdvanceSingleStep() {}
