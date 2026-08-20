# particle-sim-tech-interview

## Overview

This is an n-body problem involving particles applying force upon each other in 3D space.  As noted, this is a naive O(n^2) implementation where every particle must do a pairwise interaction with every other particle, every frame.  This will be quite computationally expensive for a large n, so optimizing for performance is imperative.

## Multi-threading

We are working with a nested loop (i, j) here where each particle loops over every other particle; the easy target for multi-threading is to split the work of the outer loop, i.e. each thread gets a range of i, and looks over all j (0..n) except itself. Force should be accumulated for each particle and applied after the threads are joined.

## General Development Notes

When reviewing the initial implementation, the first thing I've done is clean up a bit.  This code would be a bit more readable with a Vec3 type of sorts, so I've made Vec3d and implemented some basic vector operations (add, subtract, multiply/divide by scalar, length, length^2) that we may need along the way.
