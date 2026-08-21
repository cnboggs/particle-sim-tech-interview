#pragma once

#include <cmath>
#include <thread>
#include <vector>

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

	// Threaded implementation using thread-local accumulation
	// and striping of the i range for load balancing
	void UpdateThreaded(const uint32_t aNumThreads) {
		const auto n{ mParticles.size() };

		// One private n-sized buffer per thread, zero-initialized
		std::vector<std::vector<Vec3d>> buffers(aNumThreads, std::vector<Vec3d>(n));

		// Capture a thread index so each thread can jump indices in a striping fashion
		const auto accumulateWorker{ [&](const uint32_t aThreadIndex) {
			auto& buffer{ buffers[aThreadIndex] };

			// This thread is responsible for i = aThreadIndex, aThreadIndex + aNumThreads, etc.
			for (auto i{ aThreadIndex }; i < n; i += aNumThreads) {
				for (auto j{ i + 1 }; j < n; j++) {
					const auto d{ mParticles[j].mPosition - mParticles[i].mPosition };
					const auto dist{ d.Length() };
					const auto force{ mParticles[i].mMass * mParticles[j].mMass / (dist * dist * dist) };

					const auto acc{ force * d };
					buffer[i] += acc;
					buffer[j] -= acc;
				}
			}
		} };

		// Create the threads and do the accumulation of forces
		std::vector<std::thread> threads;
		threads.reserve(aNumThreads);
		for (auto threadIndex{ 0U }; threadIndex < aNumThreads; threadIndex++) {
			threads.emplace_back(accumulateWorker, threadIndex);
		}
		for (auto& thread : threads) {
			thread.join();
		}

		// Sum every thread's buffer to arrive at the actual change for each particle
		//for (const auto& buffer : buffers) {
		//	for (auto i{ 0U }; i < n; i++) {
		//		mParticles[i].mVelocity += buffer[i];
		//	}
		//}
		// Multi-threading the reduction/summation portion
		// Give each thread a contiguous range here, no worries of write conflict
		const auto reduceWorker{ [&](const uint32_t aThreadIndex) {
			const auto chunkSize{ (n + aNumThreads - 1) / aNumThreads };
			const auto start{ aThreadIndex * chunkSize };
			const auto end{ std::min(start + chunkSize, n) }; // cap the final section

			for (auto i{ start }; i < end; i++) {
				for (const auto& buffer : buffers) {
					mParticles[i].mVelocity += buffer[i];
				}
			}
		} };

		threads.clear();
		for (auto threadIndex{ 0U }; threadIndex < aNumThreads; threadIndex++) {
			threads.emplace_back(reduceWorker, threadIndex);
		}
		for (auto& thread : threads) {
			thread.join();
		}

		// Apply velocity to position
		for (auto& p : mParticles) {
			p.mPosition += p.mVelocity;
		}
	}

private:
	std::vector<Particle> mParticles;
};