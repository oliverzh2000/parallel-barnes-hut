# Parallel 3D Barnes-Hut N-Body Simulation
#### Version 2.0.0

A parallelized and efficient C++ implementation of the [Barnes-Hut algorithm](https://en.wikipedia.org/wiki/Barnes-Hut_simulation) for simulating an N-body system and visualizing it.

CUDA GPU acceleration, interactive simulation viewer, lossy simulation data compression (DCT based), and optimized support for other compilers (non-GCC) coming soon. See [Roadmap](#roadmap)

# Gallery
![](fast_demo.gif)

The above is a fast-forwarded and reduced resolution render from a previous project. Full-size 3D demo coming soon. Simulation in progress on Google Compute Engine.

# Getting Started
1. Checkout the repo from github, and build the project with `cmake`
```bash
$ git clone https://github.com/oliverzh2000/parallel-barnes-hut.git
$ cd parallel-barnes-hut
$ cmake ./ DCMAKE_BUILD_TYPE=RelWithDebinfo
$ make
$ ./nbody --help
```
See [Command Line Options](#command-line-options) for detailed usage information.

2. Initialize the simulation with between `~10^7` and `~10^8` stars with random positions and velocities and then leave the system running for a few days, weeks, or even months. See [Recommended Parameter Values](#recommended-parameter-values) for some general tips.
  - **Important!** Make sure you have enough disk space to store the simulation results. _(Coming soon)_ You can use the `--estimate` command line option to get an estimate of the disk space needed to store the simulation results at your specified level of detail as well as an estimate of the time needed to run the simulation.
3. _(Coming soon)_ Sign up for an email notification when your simulation completes.
4. _(Coming soon)_ Come back when finished and view your spectacular results on the simulation viewer.

# Command Line Options
Run the simulation driver (the `nbody` executable) with `--help` for more information. 

Use `--sim-directory` to give the simulation driver a path to a directory. This directory must contain an `init.txt` that is used to specify all the parameters of the simulation.

### Format of `init.txt`
```
integratorType: 'IntegratorLeapfrog' | 'IntegratorEuler'
    timestep: positive-decimal

forceCalcType: 'ForceCalcAllPairs' | 'ForceCalcBarnesHut' | 'ForceCalcBarnesHutParallel'
    gravConst: positive-decimal
    softening: positive-decimal
    theta: positive-decimal-value  *1.

starsInitMode: 'readStarsInline' | 'readStarsFromLatestBinaryFrame' | 'readStarsFromLatestShortFrame' |createSpiralGalaxy'
    n: number-of-stars
    pos-x pos-y pos-z vel-x vel-y vel-z mass
    ... 
    ... n rows total, one per star  *2.
    
    n: number-of-stars
    centerPos:
        x: decimal
        y: decimal
        z: decimal
    centerVel:
        x: decimal
        y: decimal
        z: decimal
    radialStdDev: positive-decimal
    avgMass: positive-decimal
    avgMassStdDev: positive-decimal
    seed: integer  *3.
```
**Notes:**
1. Use this field only with a Barnes-Hut force calculator type.
2. Use this section only with `readStarsInline` star initialization mode.
3. Use this section only with `createSpiralGalaxy` star initialization mode. 

All units are specified in SI: meters, seconds, kilograms, radian, etc.

#### Example usages of `init.txt`
```
integratorType: IntegratorLeapfrog
    timestep: 0.01

forceCalcType: ForceCalcBarnesHutParallel
    gravConst: 1.0
    softening: 0.01
    theta: 0.5

starsInitMode: readStarsFromLatestBinaryFrame
```
```
integratorType: IntegratorLeapfrog
    timestep: 0.01

forceCalcType: ForceCalcBarnesHutParallel
    gravConst: 1.0
    softening: 0.01
    theta: 0.5

starsInitMode: createSpiralGalaxy
    n: 100000
    centerPos:
        x: 0
        y: 0
        z: 0
    centerVel:
        x: 0
        y: 0
        z: 0
    radialStdDev: 100
    avgMass: 1
    avgMassStdDev: 0.1
    seed: 0
```
```
integratorType: IntegratorLeapfrog
    timestep: 0.01

forceCalcType: ForceCalcBarnesHutParallel
    gravConst: 1.0
    softening: 0.01
    theta: 0.5

starsInitMode: readStarsInline
    n: 5
    0 8 0 0 0 0 1
    5 7 0 0 0 0 1
    5 5 0 0 0 0 1
    7 5 0 0 0 0 1
    8 0 0 0 0 0 1
```
# Recommended Parameter Values
| Parameter        | Recommended value           | Reason  |
| :------------- |:-------------| :-----|
| `integratorType`      | `integratorLeapfrog` | 2nd-order compared to Euler and is time-reversible |
| `forceCalcType`    | `ForceCalcBarnesHutParallel`      | Better performance than non-parallel counterpart and the all-pairs quadratic-time algorithm is simply infeasible for large datasets |
| `theta` (if applicable) | `0.5` or `1`      | `0.5` gives very good accuracy. `1` gives acceptable accuracy at better performance |

# Roadmap

# License


