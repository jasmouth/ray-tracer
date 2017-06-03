#ifndef HITABLEH
#define HITABLEH

#include "Ray.h"

class Material;

struct HitRecord {
	float t;
	Vec3 hitPoint;
	Vec3 normal;
	Material *materialPtr;
};

class Hitable {
public:
	virtual bool Hit(const Ray& r, const float tMin, const float tMax, HitRecord& rec) const = 0;
};

#endif
