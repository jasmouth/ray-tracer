#ifndef SPHEREH
#define SPHEREH

#include "Hitable.h"

class Sphere : public Hitable {
public:
	Sphere() {};
	Sphere(Vec3 center, float r) : center(center), radius(r) {};

	virtual bool Hit(const Ray& r, const float tMin, const float tMax, HitRecord& rec) const;

	Vec3 center;
	float radius;
};

bool Sphere::Hit(const Ray& r, const float tMin, const float tMax, HitRecord& rec) const {
	Vec3 oc = r.origin - center;
	const float a = Dot(r.direction, r.direction);
	const float b = Dot(oc, r.direction);
	const float c = Dot(oc, oc) - (radius * radius);
	const float discriminant = (b * b) - (a * c);
	if (discriminant > 0) {
		float temp = (-b - sqrt(discriminant)) / a;
		if (temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.hitPoint = r.PointAtParameter(temp);
			rec.normal = (rec.hitPoint - center) / radius;
			return true;
		}
		temp = (-b + sqrt(discriminant)) / a;
		if (temp < tMax && temp > tMin) {
			rec.t = temp;
			rec.hitPoint = r.PointAtParameter(temp);
			rec.normal = (rec.hitPoint - center) / radius;
			return true;
		}
	}
	return false;
}

#endif
