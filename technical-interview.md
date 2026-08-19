### Space Simulation C++ Technical Interview

#### Question: You are working on a C++ simulation engine that requires optimization for multi-threading. The simulation involves a calculation-heavy task: Simulating a real-time particle system where each particle has properties that affect other particles within a certain range (gravity, magnetic fields, etc.). The system currently runs as a single-threaded process. Your task is to design and implement a multi-threaded version of this task and measure its performance compared to the original single-threaded version.

#### Requirements:
- The provided code illustrates a basic single threaded particle system. Each particle has
properties like position, velocity, and mass. Particles will affect each other based on their
distance and properties.
- Implement a multi-threaded version of the particle system. The particles should be
distributed among different threads for calculation.
- Ensure proper synchronization and thread safety in the multi-threaded implementation.
You should particularly consider how to handle synchronizing the particle updates.
- Benchmark both the single-threaded and multi-threaded versions for different numbers of
particles (e.g., 100, 1000, 10,000). Record the time taken for a fixed number of updates in
both cases.
- Describe any challenges or considerations when introducing multi-threading into the
engine and propose solutions to address them.
- Create a brief document or presentation that provides an overview of your solution,
including key design decisions, benchmark results, and performance analysis.
Additional Information:
- Provide comments or explanations in your code to demonstrate your thought process and
understanding of the concepts. Include all the code used in your final deliverable.
- Create a brief document or presentation to accompany your solution, explaining the
implementation details and showcasing the key aspects of your solution.
- Please provide your solution, including the particle system code, benchmarking results,
documentation or presentation, and any additional explanations or insights you deem
relevant.

The code provided represents a naive O(n^2) algorithm and will run very slowly for a large number of particles. Additionally, this code does not yet incorporate multi-threading, which is a key component of the interview task. You would need to design a strategy for dividing the particles among threads and synchronizing their updates to achieve this.

#### Bonus Question: Advanced Algorithm Integration: In addition to implementing the multi-threaded version of the particle simulation, you are now tasked with integrating the Barnes-Hut algorithm to approximate gravitational forces in a large-scale particle system. The Barnes-Hut algorithm utilizes a quadtree data structure to efficiently calculate forces between distant particles.
#### Please extend your existing solution to include the following:
- Implement a quadtree data structure to represent the spatial distribution of particles. Each
quadtree node should contain a subset of particles, and internal nodes should represent
larger regions of space.
- Modify the force calculation in the multi-threaded particle simulation to utilize the
Barnes-Hut algorithm. Instead of directly calculating forces between all particle pairs,
traverse the quadtree to approximate forces between particles that are far apart.
- Benchmark and compare the performance of the enhanced simulation (with the Barnes-
Hut algorithm) against the previous multi-threaded implementation. Measure the
execution time for a fixed number of updates for various particle system sizes and report
the performance improvements achieved.
Additional Information:
- Provide comments or explanations in your code to demonstrate your thought process and
understanding of the concepts.
- Include all the code used in your final deliverable.
- Update the brief document or presentation to describe the integration of the Barnes-Hut
algorithm, any modifications to the code, and the performance analysis.

#### Example:
Suppose you have a large-scale particle system with 10,000 particles. The previous multi-
threaded implementation without the Barnes-Hut algorithm has an average execution time of 10
seconds for 100 updates. After implementing the Barnes-Hut algorithm, the enhanced simulation
achieves an average execution time of 2 seconds for the same number of updates on the same
particle system. Provide a detailed analysis of the performance improvement and discuss any
trade-offs or limitations of the Barnes-Hut algorithm in this context.
Please extend your existing solution to incorporate the Barnes-Hut algorithm and provide the
modified code, documentation, and any additional explanations or insights you deem relevant.
