#pragma once

struct Vec3d {
	Vec3d() = default;
	explicit Vec3d(const double aX, const double aY, const double aZ)
		: mX{ aX }
		, mY{ aY }
		, mZ{ aZ }
	{
	}

	Vec3d& operator+=(const Vec3d& aOther) {
		mX += aOther.mX;
		mY += aOther.mY;
		mZ += aOther.mZ;
		return *this;
	}

	Vec3d operator+(const Vec3d& aOther) {
		Vec3d res = *this;
		res += aOther;
		return res;
	}

	Vec3d& operator-=(const Vec3d& aOther) {
		mX -= aOther.mX;
		mY -= aOther.mY;
		mZ -= aOther.mZ;
		return *this;
	}

	Vec3d operator-(const Vec3d& aOther) {
		Vec3d res = *this;
		res -= aOther;
		return res;
	}

	Vec3d& operator*=(const double aScalar) {
		mX *= aScalar;
		mY *= aScalar;
		mZ *= aScalar;
		return *this;
	}

	// friend inlines these functions outside of the class;
	// this allows us to handle scalar * vec and vec * scalar
	friend Vec3d operator*(Vec3d aVec, const double aScalar) {
		aVec *= aScalar;
		return aVec;
	}

	friend Vec3d operator*(double aScalar, const Vec3d& aVec) {
		return aVec * aScalar;
	}

	Vec3d& operator/=(const double aScalar) {
		mX /= aScalar;
		mY /= aScalar;
		mZ /= aScalar;
		return *this;
	}

	// friend inlines these functions outside of the class;
	// this allows us to handle scalar * vec and vec * scalar
	friend Vec3d operator/(Vec3d aVec, const double aScalar) {
		aVec /= aScalar;
		return aVec;
	}

	friend Vec3d operator/(double aScalar, const Vec3d& aVec) {
		return aVec / aScalar;
	}

	auto Length() const {
		return std::sqrt(mX * mX + mY * mY + mZ * mZ);
	}

	auto LengthSquared() const {
		return mX * mX + mY * mY + mZ * mZ;
	}

	double mX{ 0.0 };
	double mY{ 0.0 };
	double mZ{ 0.0 };
};