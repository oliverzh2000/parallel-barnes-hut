//
// Created by Oliver Zhang on 2019-12-20.
//

#ifndef BH_SIM_FORCECALCULATOR_H
#define BH_SIM_FORCECALCULATOR_H

class Model;

/**
 * Abstract base class for force/acceleration computation on a Model of Stars.
 */
class ForceCalc {
    double gravConst;

public:
    explicit ForceCalc(double gravConst);

    /// Update the net acceleration for each of the stars in the given Model.
    virtual void updateNetAccel(Model &model) = 0;

    /// Code to be executed after one timestep has elapsed.
    virtual void onAdvanceSingleStep() = 0;
};


#endif //BH_SIM_FORCECALCULATOR_H
