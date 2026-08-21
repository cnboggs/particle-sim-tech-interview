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
	assert(DeterminismCheck());

	//Benchmark<std::chrono::microseconds> bench{ "100 Particles, 100 Updates", cNUM_RUNS };
	//bench.Run([]() {
	//	ParticleSystem system(100U);
	//	for (auto i{ 0U }; i < cNUM_UPDATES; i++) {
	//		system.Update();
	//	}
	//	});

	//Benchmark<std::chrono::milliseconds> bench2{ "1000 Particles, 100 Updates", cNUM_RUNS };
	//bench2.Run([]() {
	//	ParticleSystem system(1000U);
	//	for (auto i{ 0U }; i < cNUM_UPDATES; i++) {
	//		system.Update();
	//	}
	//	});

	//Benchmark<std::chrono::milliseconds> bench3{ "2000 Particles, 100 Updates", cNUM_RUNS };
	//bench3.Run([]() {
	//	ParticleSystem system(2000U);
	//	for (auto i{ 0U }; i < cNUM_UPDATES; i++) {
	//		system.Update();
	//	}
	//	});

	// Automatically gather number of available threads
	//auto numThreads{ std::thread::hardware_concurrency() };
	//if (numThreads == 0)
	//{
	//	numThreads = 2;
	//}

	//Benchmark<std::chrono::milliseconds> bench{ "Multi-threaded, 1000 Particles, 100 Updates, 4 Threads", cNUM_RUNS };
	//bench.Run([numThreads=4U]() {
	//	ParticleSystem system(1000U);
	//	for (auto i{ 0U }; i < cNUM_UPDATES; i++) {
	//		system.UpdateThreaded(numThreads);
	//	}
	//	});

	//Benchmark<std::chrono::milliseconds> bench2{ "Multi-threaded, 1000 Particles, 100 Updates, 8 Threads", cNUM_RUNS };
	//bench2.Run([numThreads = 8U]() {
	//	ParticleSystem system(1000U);
	//	for (auto i{ 0U }; i < cNUM_UPDATES; i++) {
	//		system.UpdateThreaded(numThreads);
	//	}
	//	});

	//Benchmark<std::chrono::milliseconds> bench3{ "Multi-threaded, 1000 Particles, 100 Updates, 16 Threads", cNUM_RUNS };
	//bench3.Run([numThreads = 16U]() {
	//	ParticleSystem system(1000U);
	//	for (auto i{ 0U }; i < cNUM_UPDATES; i++) {
	//		system.UpdateThreaded(numThreads);
	//	}
	//	});

	//Benchmark<std::chrono::milliseconds> bench4{ "Multi-threaded, 1000 Particles, 100 Updates, 24 Threads", cNUM_RUNS };
	//bench4.Run([numThreads = 24U]() {
	//	ParticleSystem system(1000U);
	//	for (auto i{ 0U }; i < cNUM_UPDATES; i++) {
	//		system.UpdateThreaded(numThreads);
	//	}
	//	});

	return 0;
}
