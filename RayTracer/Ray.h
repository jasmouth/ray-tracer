#ifndef RAYH
#define RAYH

#include "Vec3.h"

class Ray {
public:
	Ray() {}
	Ray(const Vec3& origin, const Vec3& direction) { this->origin = origin; this->direction = direction; }

	Vec3 PointAtParameter(float t) const { return origin + t * direction; }

	Vec3 origin;
	Vec3 direction;
};

#endif
