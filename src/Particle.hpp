#pragma once

#include <utility>

#include "Vec3d.hpp"

struct Particle {
	explicit Particle(const double aMass, Vec3d aPosition, Vec3d aVelocity)
		: mMass{ aMass }
		, mPosition{ std::move(aPosition) }
		, mVelocity{ std::move(aVelocity) }
	{
	}

	double mMass;
	Vec3d  mPosition;
	Vec3d  mVelocity;
};