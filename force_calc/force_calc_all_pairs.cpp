//
// Created by Oliver Zhang on 2019-12-23.
//

#include "force_calc_all_pairs.h"
#include "../vec3d.h"
#include "../model.h"

ForceCalcAllPairs::ForceCalcAllPairs(double gravConst, double softening) : ForceCalc(gravConst, softening) {}

void ForceCalcAllPairs::updateNetAccel(Model &model) const {
    for (int i = 0; i < model.size(); ++i) {
        model.acc(i) = {0, 0, 0};
    }
    for (int i = 0; i < model.size(); ++i) {
        for (int j = 0; j < model.size(); ++j) {
            if (i != j) {
                model.acc(i) += gravFieldDueToSingleObject(model.pos(j), model.mass(j), model.pos(i));
            }
        }
    }
}

