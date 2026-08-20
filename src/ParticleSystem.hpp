#pragma once

#include <vector>
#include <cmath>

#include "Particle.hpp"

class ParticleSystem {
public:
	explicit ParticleSystem(const uint32_t aNumParticles) {
		for (auto i{ 0U }; i < aNumParticles; i++) {
			mParticles.emplace_back(rand() % 1000, rand() % 1000, rand() % 1000, 1.0, 0.0, 0.0, 0.0);
		}
	}

	void Update() {
		for (auto i{ 0U }; i < mParticles.size(); i++) {
			for (auto j{ i + 1 }; j < mParticles.size(); j++) {
				const auto dx{ mParticles[j].x - mParticles[i].x };
				const auto dy{ mParticles[j].y - mParticles[i].y };
				const auto dz{ mParticles[j].z - mParticles[i].z };
				const auto dist{ std::sqrt(dx * dx + dy * dy + dz * dz) };
				const auto force{ mParticles[i].mass * mParticles[j].mass /
					(dist * dist * dist) };

				// Apply acceleration to velocity
				// Note: force *is* acceleration here using F=ma since the
				// mass of all particles is currently hardcoded to 1.
				mParticles[i].vx += force * dx;
				mParticles[i].vy += force * dy;
				mParticles[i].vz += force * dz;
				mParticles[j].vx -= force * dx;
				mParticles[j].vy -= force * dy;
				mParticles[j].vz -= force * dz;
			}
		}

		// Apply velocity to position
		for (auto& p : mParticles) {
			p.x += p.vx;
			p.y += p.vy;
			p.z += p.vz;
		}
	}

private:
	std::vector<Particle> mParticles;
};