#ifndef HITABLEH
#define HITABLEH

#include "Ray.h"

struct HitRecord {
	float t;
	Vec3 hitPoint;
	Vec3 normal;
};

class Hitable {
public:
	virtual bool Hit(const Ray& r, const float tMin, const float tMax, HitRecord& rec) const = 0;
};

#endif
