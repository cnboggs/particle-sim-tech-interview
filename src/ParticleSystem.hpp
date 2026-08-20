#pragma once

#include <vector>
#include <cmath>

#include "Particle.hpp"

class ParticleSystem {
public:
	explicit ParticleSystem(const uint32_t aNumParticles) {
		for (auto i{ 0U }; i < aNumParticles; i++) {
			mParticles.emplace_back(1.0,
				Vec3d{ static_cast<double>(rand() % 1000), static_cast<double>(rand() % 1000), static_cast<double>(rand() % 1000) },
				Vec3d{ 0.0, 0.0, 0.0 });
		}
	}

	void Update() {
		for (auto i{ 0U }; i < mParticles.size(); i++) {
			for (auto j{ i + 1 }; j < mParticles.size(); j++) {
				const auto d{ mParticles[j].mPosition - mParticles[i].mPosition };
				const auto dist{ d.Length() };
				const auto force{ mParticles[i].mMass * mParticles[j].mMass / (dist * dist * dist) };

				// Apply acceleration to velocity
				// Note: force *is* acceleration here using F=ma since the
				// mass of all particles is currently hardcoded to 1.
				const auto acc{ force * d };
				mParticles[i].mVelocity += acc;
				mParticles[j].mVelocity -= acc;
			}
		}

		// Apply velocity to position
		for (auto& p : mParticles) {
			p.mPosition += p.mVelocity;
		}
	}

private:
	std::vector<Particle> mParticles;
};