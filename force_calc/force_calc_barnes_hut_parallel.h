//
// Created by Oliver Zhang on 2020-01-12.
//

#ifndef BH_SIM_FORCE_CALC_BARNES_HUT_PARALLEL_H
#define BH_SIM_FORCE_CALC_BARNES_HUT_PARALLEL_H

#include "force_calc_barnes_hut.h"

class ForceCalcBarnesHutParallel : public ForceCalcBarnesHut {
  public:
    explicit ForceCalcBarnesHutParallel(double gravConst, double softening, double theta);

    /// 1) Build oct-tree on one thread.
    /// 2) Perform force calculation among 8 threads. (fork and join).
    void updateNetAccel(Model &model) const override;
};

#endif // BH_SIM_FORCE_CALC_BARNES_HUT_PARALLEL_H