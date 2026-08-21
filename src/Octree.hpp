#pragma once

#include <array>
#include <memory>

#include "Particle.hpp"
#include "Vec3d.hpp"

class Octree {
public:
	Octree(const Vec3d& aCenter, const double aHalfWidth)
		: mCenter{ aCenter }
		, mHalfWidth{ aHalfWidth }
	{
	}

	void Insert(const Particle& aParticle) {
		// Every node along the insertion path accumulates mass/center of mass,
		// regardless of whether it ends up staying a leaf or becoming internal.
		const auto newMass{ mMass + aParticle.mMass };
		mCenterOfMass = (mCenterOfMass * mMass + aParticle.mPosition * aParticle.mMass) / newMass;
		mMass = newMass;

		if (!mHasParticle && !IsInternal()) {
			// Empty leaf, claim it
			mParticle = &aParticle;
			mHasParticle = true;
			return;
		}

		if (mHasParticle && !IsInternal()) {
			// Occupied leaf, subdivide and push the existing particle down first
			Subdivide();
			InsertIntoChild(*mParticle);
			mHasParticle = false;
			mParticle = nullptr;
		}

		// Internal node, recurse into whichever child this particle belongs in
		InsertIntoChild(aParticle);
	}

	Vec3d ComputeForce(const Particle& aParticle, const double aTheta) const {
		if (mMass == 0.0 || (mHasParticle && mParticle == &aParticle)) {
			// Empty node or the leaf holding aParticle itself, no self-interaction
			return Vec3d{ 0.0, 0.0, 0.0 };
		}

		const auto d{ mCenterOfMass - aParticle.mPosition };
		const auto dist{ d.Length() };

		if (mHasParticle) {
			// Leaf holding a different particle, use direct force
			const auto force{ aParticle.mMass * mMass / (dist * dist * dist) };
			return force * d;
		}

		// Internal node, use direct force
		const auto s{ mHalfWidth * 2.0 };
		if (s / dist < aTheta) {
			const auto force{ aParticle.mMass * mMass / (dist * dist * dist) };
			return force * d;
		}

		// Node is too close/large relative to distance, recurse into children
		Vec3d total{ 0.0, 0.0, 0.0 };
		for (const auto& child : mChildren) {
			if (child) {
				total += child->ComputeForce(aParticle, aTheta);
			}
		}
		return total;
	}

private:
	bool IsInternal() const { return mChildren[0] != nullptr; }

	void Subdivide() {
		const auto quarter{ mHalfWidth / 2.0 };
		for (auto octant{ 0U }; octant < 8U; octant++) {
			const Vec3d offset{
				(octant & 1U) ? quarter : -quarter,
				(octant & 2U) ? quarter : -quarter,
				(octant & 4U) ? quarter : -quarter
			};
			mChildren[octant] = std::make_unique<Octree>(mCenter + offset, quarter);
		}
	}

	void InsertIntoChild(const Particle& aParticle) {
		mChildren[GetOctant(aParticle.mPosition)]->Insert(aParticle);
	}

	uint32_t GetOctant(const Vec3d& aPosition) const {
		uint32_t octant{ 0U };
		if (aPosition.mX >= mCenter.mX) octant |= 1U;
		if (aPosition.mY >= mCenter.mY) octant |= 2U;
		if (aPosition.mZ >= mCenter.mZ) octant |= 4U;
		return octant;
	}

	Vec3d                                  mCenter{ 0.0, 0.0, 0.0 };
	double                                 mHalfWidth{ 0.0 };
	double                                 mMass{ 0.0 };
	Vec3d                                  mCenterOfMass{ 0.0, 0.0, 0.0 };
	bool								   mHasParticle{ false };
	const Particle*						   mParticle{ nullptr };
	std::array<std::unique_ptr<Octree>, 8> mChildren;
};
