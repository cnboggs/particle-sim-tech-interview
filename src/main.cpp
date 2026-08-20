#include <iostream>

#include "Particle.hpp"
#include "ParticleSystem.hpp"

int main() {
	const auto cNUM_PARTICLES{ 100U };
	const auto cNUM_UPDATES{ 100U };

	ParticleSystem system(cNUM_PARTICLES);
	for (auto i{ 0U }; i < cNUM_UPDATES; i++) {
		system.Update();

		std::cout << i << std::endl;
	}

	return 0;
}
