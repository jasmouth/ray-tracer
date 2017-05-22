#include <fstream>
#include <chrono>
#include <random>
#include <limits>
#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"

Vec3 Color(const Ray& r, HitableList *world);

Vec3 RandomPointInUnitSphere();

int main() {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 generator(seed);
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

	int numX = 200;
	int numY = 100;
	int numSamples = 100;
	Hitable *list[2];
	list[0] = new Sphere(Vec3(0.0f, 0.0f, -1.0f), 0.5f);
	list[1] = new Sphere(Vec3(0.0f, -100.5f, -1.0f), 100.0f);
	HitableList *world = new HitableList(list, 2);
	Camera camera;

	std::ofstream outputFile;
	outputFile.open("output.ppm");
	outputFile << "P3\n" << numX << " " << numY << "\n255\n";
	for (int j = numY - 1; j >= 0; j--) {
		for (int i = 0; i < numX; i++) {
			Vec3 col(0.0f, 0.0f, 0.0f);
			for (int s = 0; s < numSamples; s++) {
				const float u = float(i + distribution(generator)) / float(numX);
				const float v = float(j + distribution(generator)) / float(numY);
				Ray r = camera.CreateRay(u, v);
				col += Color(r, world);
			}
			col /= float(numSamples);
			col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			const int ir = int(255.99 * col[0]);
			const int ig = int(255.99 * col[1]);
			const int ib = int(255.99 * col[2]);
			outputFile << ir << " " << ig << " " << ib << "\n";
		}
	}
	outputFile.close();
	return 0;
}

Vec3 Color(const Ray& r, HitableList *world) {
	HitRecord rec;
	if (world->Hit(r, 0.001f, std::numeric_limits<float>::max(), rec)) {
		Vec3 target = rec.hitPoint + rec.normal + RandomPointInUnitSphere();
		return 0.5f * Color(Ray(rec.hitPoint, target - rec.hitPoint), world);
	}
	else {
		Vec3 unitDirection = UnitVector(r.direction);
		const float t = 0.5f * (unitDirection.y() + 1.0f);
		// Linear Interpolation
		// blendedValue = (1-t)*startValue + t*endValue => where 0.0 < t < 1.0
		return (1.0f - t)*Vec3(1.0f, 1.0f, 1.0f) + t*Vec3(0.5f, 0.7f, 1.0f);
	}
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
