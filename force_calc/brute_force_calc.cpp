//
// Created by Oliver Zhang on 2019-12-23.
//

#include "brute_force_calc.h"
#include "../vec3d.h"
#include "../model.h"

BruteForceCalc::BruteForceCalc(double gravConst) : ForceCalc(gravConst) {}

void BruteForceCalc::updateNetAccel(Model &model) {
    for (int i = 0; i < model.size(); ++i) {
        model.acc(i) = {0, 0, 0};
    }
    for (int i = 0; i < model.size(); ++i) {
        for (int j = 0; j < model.size(); ++j) {
            if (i != j) {
                double r = model.pos(i).distanceTo(model.pos(j));
                model.acc(i) += (model.pos(j) - model.pos(i)) * (gravConst * model.mass(j) / r * r * r);
            }
        }
    }
}

void BruteForceCalc::afterAdvanceSingleStep() {}
