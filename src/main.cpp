#include <iostream>

#include "Particle.hpp"
#include "ParticleSystem.hpp"

int main() {
	const int NUM_PARTICLES = 100;
	const int NUM_UPDATES = 100;
	ParticleSystem system(NUM_PARTICLES);
	for (int i = 0; i < NUM_UPDATES; i++) {
		system.update();
	}
	return 0;
}
