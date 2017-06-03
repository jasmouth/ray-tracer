#ifndef MATERIALH
#define MATERIALH

struct HitRecord;

#include "Ray.h"
#include "Hitable.h"

Vec3 RandomPointInUnitSphere() {
	Vec3 p;
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 generator(seed);
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);
	do {
		p = 2.0f * Vec3(dist(generator), dist(generator), dist(generator)) - Vec3(1.0f, 1.0f, 1.0f);
	} while (p.SquaredLength() >= 1.0f);
	return p;
}

Vec3 Reflect(const Vec3& v, const Vec3& n) {
	return v - (2 * Dot(v, n) * n);
}

class Material {
public:
	virtual bool Scatter(const Ray& inputRay, const HitRecord& rec, Vec3& attenuation, Ray& scatteredRay) const = 0;
};

class Lambertian : public Material {
public:
	Lambertian(const Vec3& albedo) : albedo(albedo) {};

	virtual bool Scatter(const Ray& inputRay, const HitRecord& rec, Vec3& attenuation, Ray& scatteredRay) const {
		Vec3 target = rec.hitPoint + rec.normal + RandomPointInUnitSphere();
		scatteredRay = Ray(rec.hitPoint, target - rec.hitPoint);
		attenuation = albedo;
		return true;
	}

	Vec3 albedo;
};

class Metal : public Material {
public:
	Metal(const Vec3& albedo, const float f) : albedo(albedo) {	fuzziness = f < 1.0f ? f : 1.0f; };

	virtual bool Scatter(const Ray& inputRay, const HitRecord& rec, Vec3& attenuation, Ray& scatteredRay) const {
		Vec3 reflected = Reflect(UnitVector(inputRay.direction), rec.normal);
		scatteredRay = Ray(rec.hitPoint, reflected + fuzziness * RandomPointInUnitSphere());
		attenuation = albedo;
		return Dot(scatteredRay.direction, rec.normal) > 0;
	}

	Vec3 albedo;
	float fuzziness;
};

#endif
