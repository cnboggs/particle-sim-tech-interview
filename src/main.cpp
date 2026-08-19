#include <iostream>
#include <vector>
#include <cmath>

struct Particle {
	double x, y, z;
	double mass;
	double vx, vy, vz;
};

class ParticleSystem {
public:
	std::vector<Particle> particles;
	ParticleSystem(int num_particles) {
		for (int i = 0; i < num_particles; i++) {
			Particle p = { rand() % 1000, rand() % 1000, rand() % 1000,
			1.0,
			0.0, 0.0, 0.0 };
			particles.push_back(p);
		}
	}
	void update() {
		for (int i = 0; i < particles.size(); i++) {
			for (int j = i + 1; j < particles.size(); j++) {
				double dx = particles[j].x - particles[i].x;
				double dy = particles[j].y - particles[i].y;
				double dz = particles[j].z - particles[i].z;
				double dist = std::sqrt(dx * dx + dy * dy + dz * dz);
				double force = particles[i].mass * particles[j].mass /
					(dist * dist * dist);
				particles[i].vx += force * dx;
				particles[i].vy += force * dy;
				particles[i].vz += force * dz;
				particles[j].vx -= force * dx;
				particles[j].vy -= force * dy;
				particles[j].vz -= force * dz;
			}
		}
		for (auto& p : particles) {
			p.x += p.vx;
			p.y += p.vy;
			p.z += p.vz;
		}
	}
};

int main() {
	const int NUM_PARTICLES = 100;
	const int NUM_UPDATES = 100;
	ParticleSystem system(NUM_PARTICLES);
	for (int i = 0; i < NUM_UPDATES; i++) {
		system.update();
	}
	return 0;
}
