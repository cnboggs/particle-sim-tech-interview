#include "Benchmark.hpp"
#include "Particle.hpp"
#include "ParticleSystem.hpp"

namespace
{
	constexpr auto cNUM_UPDATES{ 100U };
	constexpr auto cNUM_RUNS{ 10U };
}

int main() {

	Benchmark<std::chrono::microseconds> bench{ "100 Particles, 100 Updates", cNUM_RUNS };
	bench.Run([]() {
		ParticleSystem system(100U);
		for (auto i{ 0U }; i < cNUM_UPDATES; i++) {
			system.Update();
		}
		});

	Benchmark<std::chrono::milliseconds> bench2{ "1000 Particles, 100 Updates", cNUM_RUNS };
	bench2.Run([]() {
		ParticleSystem system(1000U);
		for (auto i{ 0U }; i < cNUM_UPDATES; i++) {
			system.Update();
		}
		});

	Benchmark<std::chrono::milliseconds> bench3{ "2000 Particles, 100 Updates", cNUM_RUNS };
	bench2.Run([]() {
		ParticleSystem system(2000U);
		for (auto i{ 0U }; i < cNUM_UPDATES; i++) {
			system.Update();
		}
		});

	return 0;
}
