//
// Created by Oliver Zhang on 2020-01-02.
//

#ifndef BH_SIM_FORCECALCBARNESHUT_H
#define BH_SIM_FORCECALCBARNESHUT_H


#include "force_calc.h"
#include "octree.h"

class BreadthFirstOctree;

class ForceCalcBarnesHut : public  ForceCalc {
    double theta; // Max angular threshold for a center of mass to not recursive split up upon force calculation.
public:
    explicit ForceCalcBarnesHut(double gravConst, double softening, double theta);

    /// Builds and traverses an OctTree.
    void updateNetAccel(Model &model) const override;

protected:
    virtual Vec3D gravFieldViaTree(const Octree::Node &node, double length, const Vec3D &pos) const;
    virtual Vec3D gravFieldViaTree2(const BreadthFirstOctree &tree, int node, double length, const Vec3D &pos) const;
};


#endif //BH_SIM_FORCECALCBARNESHUT_H
