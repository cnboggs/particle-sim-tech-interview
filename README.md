# particle-sim-tech-interview

## Overview

This is an n-body problem involving particles applying force upon each other in 3D space.  As noted, this is a naive O(n^2) implementation where every particle must do a pairwise interaction with every other particle, every frame.  This will be quite computationally expensive for a large n, so optimizing for performance is imperative.

## General Development Notes

When reviewing the initial implementation, the first thing I've done is clean up a bit.  This code would be a bit more readable with a Vec3 type of sorts, so I've made Vec3d and implemented some basic vector operations (add, subtract, multiply/divide by scalar, length, length^2) that we may need along the way.

I decided before starting on the multi-threading solution that it would be a good idea to brush up on a basic benchmarking setup.  I created a basic Benchmark class that will allow me to measure the execution of any block of code over a number of runs. I added the ability to tweak units, which may prove useful when testing different sizes of n.

Testing the approach currently in place, I got the following benchmark results:

| Benchmark | Runs | Total (ms) | Average (ms) | Min (ms) | Max (ms) |
|-|-|-|-|-|-|
| 100 Particles, 100 Updates  | 10 | 328.865 | **32.886** | 32.070 | 37.817 |
| 1000 Particles, 100 Updates | 10 | 32878   | **3287**   | 3235   | 3321   |
| 2000 Particles, 100 Updates | 10 | 129982  | **12998**  | 12988  | 13015  |

To confirm the O(n^2) behavior we can compare the change in particles to the change in average execution time. A 10x change in the number of particles should result in a 100x change in execution time:

| Transition | Particle Scale | Avg Time Before -> After | Expected Time Scale | Actual Time Scale |
|-|-|-|-|-|
| 100 -> 1000 | 10x | 32.886 ms -> 3287 ms | 100x | **~99.96x** |
| 1000 -> 2000 | 2x | 3287 ms -> 12998 ms | 4x | **~3.95x** |

This lines up pretty nicely. Next is implementing a multi-threading approach to compare.

## Multi-threading

We are working with a triangular nested loop: i ranges 0..n, and j ranges i+1..n. This can be done due to Newton's third law -- the force particle i exerts on j is equal and opposite to the force j exerts on i. Each pair is computed once instead of twice, and the result is applied to both particles' velocities at the same time.  This saves from duplicating calculations unnecessarily, but also introduces some challenges when it comes to multi-threading effectively.

There are two challenges to solve here:
 - **Write conflict** - Currently each Update() applies force from i to j and j to i in the same loop, and each particle's new velocity for a given update is not determined until it has accumulated force from every other particle. If we naively split the loop up into threads without addressing this we'd cause a data race on the shared particle state.

 - **Load balancing** - The way the loops are set up to symmetrize the calculations causes a load balancing issue.  With a loop of i..n, and j=i+i..n, as i increases the inner loop j has less work to do (i.e., a triangular shape).  If we split the work over sections of i we'd be giving the earlier threads **much** more work to do than the later threads.

### Write conflict

There are a few solutions that I see here to resolve the write conflict:

 - **Drop the symmetry optimization** - Convert back to a full i..n, j..n where j != i loop, where we can easily give each thread its own i range, and the write conflict disappears. We're doubling the computation for easy synchronization.

 - **Thread-local accumulation** - Each thread gets its own buffer for accumulating force on all particles, so that write conflicts do not occur.  Once the work has been done, we can sum these forces and reduce them into a single value that we then apply to each particle.  The cost here is memory usage.

 - **Use std::atomic** - Each particle's velocity could be wrapped in an atomic. I don't see this being very effective as threads will very likely run into having to wait on each other; this is probably not worth implementing.

### Load balancing

For balancing the load across threads, I can think of two options:

 - **Striped i range instead of contiguous i range** - Easy to get a coarsely balanced amount of work between threads for the triangular loop; we could stripe the assignment of i values for each thread rather than giving each of them a contiguous i range.  That is, if we have 4 threads  the first thread gets i=0,4,8,12,... the second gets i=1,5,9,13,... and so on. This would spread the biggest available i to each thread. Note that this is still biased towards earlier threads, but this imbalance shrinks as n grows.

 - **Calculate contiguous ranges** - Determine a method of computing range boundaries so that each thread is doing an equal amount of work.  This trades more computation for more accurate load balancing.

## Implementing the multi-threading

I decided to go with thread-local accumulation to resolve the write conflict, and striping the i range for the load balancing. I ran the multi-threaded solution over 1000 particles with different thread counts to get an idea of what I was working with:

| Benchmark (n=1000, 100 Updates) | Runs | Total (ms) | Average (ms) | Min (ms) | Max (ms) |
|-|-|-|-|-|-|
| 1 Thread  (Initial benchmark)| 10 | 32878 | **3287** | 3235 | 3321 |
| 4 Threads | 10 | 9980 | **998** | 977 | 1061 |
| 8 Threads | 10 | 10044 | **1004** | 956 | 1093 |
| 16 Threads | 10 | 10298 | **1029** | 995 | 1056 |
| 24 Threads | 10 | 14891 | **1489** | 1407 | 1539 |

Multi-threading cuts down the runtime significantly.  One thing I immediately noticed was that as the thread count increased the performance plateaud and past a point performed worse.  This could be due to the overhead of managing the threads themselves, or requesting all of the threads fighting the OS. I was able to confirm thread overhead performance impact when I added multi-threading to the reduction portion of my update that was previously single-threaded (summing values from all buffers); things ran significantly slower on average:

| Benchmark (n=1000, 100 Updates) | Runs | Total (ms) | Average (ms) | Min (ms) | Max (ms) |
|-|-|-|-|-|-|
| 1 Thread (Initial benchmark) | 10 | 32878 | **3287** | 3235 | 3321 |
| 4 Threads | 10 | 15219 | **1521** | 1231 | 1645 |
| 8 Threads | 10 | 12768 | **1276** | 1264 | 1302 |
| 16 Threads | 10 | 19834 | **1983** | 1797 | 2116 |
| 24 Threads | 10 | 29482 | **2948** | 2881 | 2987 |

This is something that would typically be resolved with a thread pool or the new C++20 barrier setup, however I will not be exploring this.  I'll leave the reduction portion single-threaded for now.

## Determinism

It's worth noting that all of the benchmarks I've run so far have been in Debug configuration, so we may be losing out on some compiler optimizations; also, the randomization of the particle positions using rand() % 1000 in unseeded fashion doesn't lend itself to accurate benchmark comparison as different computations are being done for each ParticleSystem.  I've implemented a seed setup with a Mersenne twister to get deterministic results.  I threw together a small check for this in my main.cpp to confirm.  Also, going forward I'll benchmark in Release.

## Barnes-Hut Algorithm

The Barnes-Hut Algorithm is a way of approximating forces on a particle by splitting them up into regions via a quadtree in 2D space or an octree in 3D space, and using the center of mass of a region to get the applied force. This reduces the calculations required for an n-body simulation from the O(n^2) time we've worked with so far to an O(nlogn) time, which is a significant improvement for large n; this comes at a cost of accuracy which we'll look at when we benchmark.

Do note that the question talks about quadtrees which would be 2D space, however the particle system as it stands is 3D which would call for an octree implementation; this is what I'll implement.

Octree's can be represented by a tree-like structure of nodes. Each of these nodes represents a region, and may contain 0 or 1 particles.  The tree is constructed one particle at a time -- as each particle is added, if a collision occurs, a node will subdivide and place both particles that have collided while keeping track of the center of mass for each region.

I will not go into full detail on how the algorithm works as it's out of scope and there are many great resources online for this, such as:
- https://arborjs.org/docs/barnes-hut
- https://lewiscoleblog.com/barnes-hut

The θ value used in the Barnes-Hut algorithm can be used to control how accurate we want the calculations; a lower value will invoke more calculations and will be more accurate, and a higher value will do the inverse.  The trade-off to make here is accuracy versus speed.  I've compared a few different theta values to the original brute-force approach, both single-threaded (Benchmarking in Release now):

| Benchmark (n=100, 100 Updates) | Runs | Total (ms) | Average (ms) | Min (ms) | Max (ms) |
|-|-|-|-|-|-|
| Single-threaded | 10 | 20 | 2 | 2 | 2 |
| Barnes-Hut theta=0.5 | 10 | 117 | 11 | 11 | 14 |
| Barnes-Hut theta=1.0 | 10 | 71 | 7 | 5 | 8 |
| Barnes-Hut theta=1.5 | 10 | 52 | 5 | 5 | 6 |

| Benchmark (n=1000, 100 Updates) | Runs | Total (ms) | Average (ms) | Min (ms) | Max (ms) |
|-|-|-|-|-|-|
| Single-threaded | 10 | 2548 | 254 | 252 | 260 |
| Barnes-Hut theta=0.5 | 10 | 5352 | 535 | 521 | 553 |
| Barnes-Hut theta=1.0 | 10 | 1945 | 194 | 188 | 204 |
| Barnes-Hut theta=1.5 | 10 | 1445 | 144 | 142 | 147 |

| Benchmark (n=10000, 100 Updates) | Runs | Total (ms) | Average (ms) | Min (ms) | Max (ms) |
|-|-|-|-|-|-|
| Single-threaded | 10 | 261217 | 26121 | 25433 | 26565 |
| Barnes-Hut theta=0.5 | 10 | 181101 | 18110 | 17817 | 18469 |
| Barnes-Hut theta=1.0 | 10 | 70691 | 7069 | 7008 | 7177 |
| Barnes-Hut theta=1.5 | 10 | 35488 | 3548 | 3461 | 3691 |

Looking the performance gains:

| Benchmark (n=10000, 100 Updates) | Average (ms) | Speedup | % Faster |
|-|-|-|-|
| Single-threaded | 26121 | 1.00x | — |
| Barnes-Hut theta=0.5 | 18110 | 1.44x | +44.2% |
| Barnes-Hut theta=1.0 | 7069 | 3.70x | +269.5% |
| Barnes-Hut theta=1.5 | 3548 | 7.36x | +636.2% |

The Barnes-Hut approach seems to hurt at low n values, and really starts to shine as n gets larger.
