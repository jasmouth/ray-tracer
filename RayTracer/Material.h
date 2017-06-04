#ifndef MATERIALH
#define MATERIALH

struct HitRecord;

#include "Ray.h"
#include "Hitable.h"

Vec3 Reflect(const Vec3& v, const Vec3& n) {
	return v - (2 * Dot(v, n) * n);
}

bool Refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refractedRay) {
	Vec3 uv = UnitVector(v);
	const float dt = Dot(uv, n);
	const float discriminant = 1.0f - (ni_over_nt * ni_over_nt * (1 - dt * dt));
	if (discriminant > 0) {
		refractedRay = (ni_over_nt * (uv - n * dt)) - (n * sqrt(discriminant));
		return true;
	} else {
		return false;
	}
}

float Schlick(float cosine, const float refractiveIndex) {
	float r0 = (1 - refractiveIndex) / (1 + refractiveIndex);
	r0 = r0 * r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}

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

class Dielectric : public Material {
public:
	Dielectric(const float ri) : refractiveIndex(ri) {};

	virtual bool Scatter(const Ray& inputRay, const HitRecord& rec, Vec3& attenuation, Ray& scatteredRay) const {
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::mt19937 generator(seed);
		std::uniform_real_distribution<float> dist(0.0f, 1.0f);

		Vec3 outwardNormal;
		Vec3 reflected = Reflect(inputRay.direction, rec.normal);
		float ni_over_nt;
		attenuation = Vec3(1.0f, 1.0f, 1.0f);
		float reflectProbability;
		float cosine;
		const float dotProd = Dot(inputRay.direction, rec.normal);
		if (dotProd > 0) {
			outwardNormal = -rec.normal;
			ni_over_nt = refractiveIndex;
			cosine = refractiveIndex * dotProd / inputRay.direction.Length();
		} else {
			outwardNormal = rec.normal;
			ni_over_nt = 1.0f / refractiveIndex;
			cosine = -dotProd / inputRay.direction.Length();
		}
		Vec3 refracted;
		if (Refract(inputRay.direction, outwardNormal, ni_over_nt, refracted)) {
			reflectProbability = Schlick(cosine, refractiveIndex);
		} else {
			reflectProbability = 1.0f;
		}

		if (dist(generator) < reflectProbability) {
			scatteredRay = Ray(rec.hitPoint, reflected);
		} else {
			scatteredRay = Ray(rec.hitPoint, refracted);
		}

		return true;
	}

	const float refractiveIndex;
};

#endif
