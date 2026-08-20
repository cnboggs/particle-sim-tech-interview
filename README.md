# particle-sim-tech-interview

## Overview

This is an n-body problem involving particles applying force upon each other in 3D space.  As noted, this is a naive O(n^2) implementation where every particle must do a pairwise interaction with every other particle, every frame.  This will be quite computationally expensive for a large n, so optimizing for performance is imperative.

## Multi-threading

We are working with a nested loop (i, j) here where each particle loops over every other particle; the easy target for multi-threading is to split the work of the outer loop, i.e. each thread gets a range of i, and looks over all j (0..n) except itself. Force should be accumulated for each particle and applied after the threads are joined.

## General Development Notes

When reviewing the initial implementation, the first thing I've done is clean up a bit.  This code would be a bit more readable with a Vec3 type of sorts, so I've made Vec3d and implemented some basic vector operations (add, subtract, multiply/divide by scalar, length, length^2) that we may need along the way.

I decided before starting on the multi-threading solution that it would be a good idea to brush up on a basic benchmarking setup.  I created a basic Benchmark class that will allow me to measure the execution of any block of code over a number of runs. I added the ability to tweak units, which may prove useful when testing different sizes of n.

Testing the approach currently in place, I got the following benchmark results:

| Benchmark | Runs | Total (ms) | Average (ms) | Min (ms) | Max (ms) |
|-|-|-|-|-|-|
| 100 Particles, 100 Updates  | 10 | 328.865 | 32.886 | 32.070 | 37.817 |
| 1000 Particles, 100 Updates | 10 | 32878   | 3287   | 3235   | 3321   |
| 2000 Particles, 100 Updates | 10 | 129982  | 12998  | 12988  | 13015  |

To confirm the O(n^2) behavior we can compare the change in particles to the change in average execution time. A 10x change in the number of particles should result in a 100x change in execution time:

| Transition | Particle Scale | Avg Time Before -> After | Expected Time Scale | Actual Time Scale |
|-|-|-|-|-|
| 100 -> 1000 | 10x | 32.886 ms -> 3287 ms | 100x | ~99.96x |
| 1000 -> 2000 | 2x | 3287 ms -> 12998 ms | 4x | ~3.95x |

This lines up pretty nicely. Next is implementing a multi-threading approach to compare.
