#ifndef CAMERAH
#define CAMERAH

#include "Ray.h"

class Camera {
public:
	Camera() {
		origin = Vec3(0.0f, 0.0f, 0.0f);
		lowerLeftCorner = Vec3(-2.0f, -1.0f, -1.0f);
		horizontal = Vec3(4.0f, 0.0f, 0.0f);
		vertical = Vec3(0.0f, 2.0f, 0.0f);
	}
	Ray CreateRay(const float u, const float v) const {
		return Ray(origin, lowerLeftCorner + u*horizontal + v*vertical);
	}

	Vec3 origin;
	Vec3 lowerLeftCorner;
	Vec3 horizontal;
	Vec3 vertical;
};

#endif
