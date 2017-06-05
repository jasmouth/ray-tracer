#ifndef CAMERAH
#define CAMERAH

#include "Ray.h"

Vec3 RandomPointInUnitDisk() {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 generator(seed);
	std::uniform_real_distribution<float> dist(0.0f, 1.0f);
	Vec3 p;
	do {
		p = 2.0f * Vec3(dist(generator), dist(generator), 0.0f) - Vec3(1.0f, 1.0f, 0.0f);
	} while (Dot(p, p) >= 1.0f);
	return p;
}

class Camera {
public:
	Camera(
			const Vec3 lookFrom,
			const Vec3 lookAt,
			const Vec3 viewUp,
			const float verticalFOV,
			const float aspectRatio,
			const float aperture,
			const float focusDist
	) {
		lensRadius = aperture / 2.0f;
		float theta = verticalFOV * M_PI/180;
		float halfHeight = tan(theta/2);
		float halfWidth = aspectRatio * halfHeight;
		origin = lookFrom;
		w = UnitVector(lookFrom - lookAt);
		u = UnitVector(Cross(viewUp, w));
		v = Cross(w, u);
		lowerLeftCorner = origin - (halfWidth * focusDist * u) - (halfHeight * focusDist * v) - (focusDist * w);
		horizontal = 2 * halfWidth * focusDist * u;
		vertical = 2 * halfHeight * focusDist * v;
	}

	Ray CreateRay(const float s, const float t) const {
		const Vec3 randomPoint = lensRadius * RandomPointInUnitDisk();
		const Vec3 offset = (u * randomPoint.x()) + (v * randomPoint.y());
		return Ray(origin + offset, lowerLeftCorner + (s * horizontal) + (t * vertical) - origin - offset);
	}

	Vec3 origin;
	Vec3 lowerLeftCorner;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 u, v, w;
	float lensRadius;
};

#endif
