#pragma once
#include <math.h>
#include <stdlib.h>
#include <iostream>

class Vec3 {
public:
	Vec3() {}
	Vec3(const float e0, const float e1, const float e2) { e[0] = e0; e[1] = e1; e[2] = e2; }

	inline float x() const { return e[0]; }
	inline float y() const { return e[1]; }
	inline float z() const { return e[2]; }
	inline float r() const { return e[0]; }
	inline float g() const { return e[1]; }
	inline float b() const { return e[2]; }

	inline const Vec3& operator+() const { return *this; }
	inline Vec3 operator-() const { return Vec3(-e[0], -e[1], -e[2]); }
	inline float operator[](int i) const { return e[i]; }
	inline float& operator[](int i) { return e[i]; }

	inline Vec3& operator+=(const Vec3 &v2);
	inline Vec3& operator-=(const Vec3 &v2);
	inline Vec3& operator*=(const Vec3 &v2);
	inline Vec3& operator/=(const Vec3 &v2);
	inline Vec3& operator*=(const float c);
	inline Vec3& operator/=(const float c);

	inline float Length() const { return sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]); }
	inline float SquaredLength() const { return e[0]*e[0] + e[1]*e[1] + e[2]*e[2]; }
	inline void MakeUnitVector();

	float e[3];
};

inline std::istream& operator>>(std::istream &istream, Vec3 &v) {
	istream >> v.e[0] >> v.e[1] >> v.e[2];
	return istream;
}

inline std::ostream& operator<<(std::ostream &ostream, const Vec3 &v) {
	ostream << v.e[0] << v.e[1] << v.e[2];
	return ostream;
}

inline void Vec3::MakeUnitVector() {
	const float k = 1.0f / sqrt(e[0]*e[0] + e[1]*e[1] + e[2]*e[2]);
	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
}

inline Vec3 operator+(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2] + v2.e[2]);
}

inline Vec3 operator-(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline Vec3 operator*(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline Vec3 operator/(const Vec3 &v1, const Vec3 &v2) {
	return Vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline Vec3 operator*(float c, const Vec3 &v) {
	return Vec3(c * v.e[0], c * v.e[1], c * v.e[2]);
}

inline Vec3 operator/(Vec3 v, float c) {
	return Vec3(v.e[0] / c, v.e[1] / c, v.e[2] / c);
}

inline Vec3 operator*(const Vec3 &v, float c) {
	return Vec3(c * v.e[0], c * v.e[1], c * v.e[2]);
}

inline Vec3& Vec3::operator+=(const Vec3 &v) {
	e[0] += v.e[0];
	e[1] += v.e[1];
	e[2] += v.e[2];
	return *this;
}

inline Vec3& Vec3::operator-=(const Vec3 &v) {
	e[0] -= v.e[0];
	e[1] -= v.e[1];
	e[2] -= v.e[2];
	return *this;
}

inline Vec3& Vec3::operator*=(const Vec3 &v) {
	e[0] *= v.e[0];
	e[1] *= v.e[1];
	e[2] *= v.e[2];
	return *this;
}

inline Vec3& Vec3::operator/=(const Vec3 &v) {
	e[0] /= v.e[0];
	e[1] /= v.e[1];
	e[2] /= v.e[2];
	return *this;
}

inline Vec3& Vec3::operator*=(const float c) {
	e[0] *= c;
	e[1] *= c;
	e[2] *= c;
	return *this;
}

inline Vec3& Vec3::operator/=(const float c) {
	const float k = 1.0f / c;

	e[0] *= k;
	e[1] *= k;
	e[2] *= k;
	return *this;
}

inline Vec3 UnitVector(Vec3 v) {
	return v / v.Length();
}

inline float Dot(const Vec3 &v1, const Vec3 &v2) {
	return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}

inline Vec3 Cross(const Vec3 &v1, const Vec3 &v2) {
	return Vec3( (v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
				(-(v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0])),
				(v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
}
