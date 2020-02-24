//
// Created by Oliver Zhang on 2020-02-23.
//

#include "catch.hpp"

#include "../base/model.h"
#include "../force_calc/oct_tree.h"

int theAnswer() {
    return 6 * 9;
}

TEST_CASE( "Life, the universe and everything", "[42][theAnswer]" ) {
    REQUIRE( theAnswer() == 54 );
}

TEST_CASE("barnes-hut force calculation") {
    Model model;
//    0 8 0 0 0 0 1
//    5 7 0 0 0 0 1
//    5 5 0 0 0 0 1
//    7 5 0 0 0 0 1
//    8 0 0 0 0 0 1
    model.addStar({0, 8, 0}, {0, 0, 0}, 1);
    model.addStar({5, 7, 0}, {0, 0, 0}, 1);
    model.addStar({5, 5, 0}, {0, 0, 0}, 1);
    model.addStar({7, 5, 0}, {0, 0, 0}, 1);
    model.addStar({8, 0, 0}, {0, 0, 0}, 1);

    OctTree octTree{model};

    SECTION("basic") {
        REQUIRE(!octTree.root.isLeaf());
        REQUIRE(!octTree.root.isEmpty());
    }
}
