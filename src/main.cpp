#include <array>
#include <cassert>
#include <iostream>
#include <thread>

#include "Benchmark.hpp"
#include "Particle.hpp"
#include "ParticleSystem.hpp"

namespace
{
	constexpr auto cNUM_UPDATES{ 100U };
	constexpr auto cNUM_RUNS{ 10U };
	constexpr auto cTHETA{ 1.0 };
	constexpr std::array cPARTICLE_COUNTS{ 100U, 1000U, 10000U };
	constexpr std::array cTHREAD_COUNTS{ 4U, 8U, 16U };

	bool ParticlesApproxEqual(const std::vector<Particle>& aLhs, const std::vector<Particle>& aRhs, const double aEpsilon = 1e-6) {
		if (aLhs.size() != aRhs.size()) return false;
		for (auto i{ 0U }; i < aLhs.size(); i++) {
			if ((aLhs[i].mPosition - aRhs[i].mPosition).Length() > aEpsilon) return false;
			if ((aLhs[i].mVelocity - aRhs[i].mVelocity).Length() > aEpsilon) return false;
		}
		return true;
	}

	bool DeterminismCheck() {
		ParticleSystem naive{ 100U };
		ParticleSystem threaded{ 100U };

		for (auto i{ 0U }; i < cNUM_UPDATES; i++) {
			naive.Update();
			threaded.UpdateThreaded(4U);
		}

		return ParticlesApproxEqual(naive.GetParticles(), threaded.GetParticles());
	}
}

int main() {
	//assert(DeterminismCheck());

	for (const auto n : cPARTICLE_COUNTS) {
		Benchmark<std::chrono::milliseconds> naiveBench{ std::format("Naive, {} Particles", n), cNUM_RUNS };
		naiveBench.Run([n]() {
			ParticleSystem system(n);
			for (auto i{ 0U }; i < cNUM_UPDATES; i++) system.Update();
			});

		Benchmark<std::chrono::milliseconds> bhBench{ std::format("Barnes-Hut, {} Particles, theta={}", n, cTHETA), cNUM_RUNS };
		bhBench.Run([n]() {
			ParticleSystem system(n);
			for (auto i{ 0U }; i < cNUM_UPDATES; i++) system.UpdateBarnesHut(cTHETA);
			});

		for (const auto threads : cTHREAD_COUNTS) {
			Benchmark<std::chrono::milliseconds> naiveThreadedBench{ std::format("Naive Threaded, {} Particles, {} Threads", n, threads), cNUM_RUNS };
			naiveThreadedBench.Run([n, threads]() {
				ParticleSystem system(n);
				for (auto i{ 0U }; i < cNUM_UPDATES; i++) system.UpdateThreaded(threads);
				});

			Benchmark<std::chrono::milliseconds> bhThreadedBench{ std::format("Barnes-Hut Threaded, {} Particles, {} Threads, theta={}", n, threads, cTHETA), cNUM_RUNS };
			bhThreadedBench.Run([n, threads]() {
				ParticleSystem system(n);
				for (auto i{ 0U }; i < cNUM_UPDATES; i++) system.UpdateBarnesHutThreaded(threads, cTHETA);
				});
		}
	}

	return 0;
}
