#include <fstream>
#include <chrono>
#include <random>
#include <limits>
#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"
#include "Material.h"

Vec3 Color(const Ray& r, HitableList *world, int depth);

int main() {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 generator(seed);
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

	int numX = 800;
	int numY = 400;
	int numSamples = 100;
	Hitable *list[4];
	list[0] = new Sphere(Vec3(0.0f, 0.0f, -1.0f), 0.5f, new Lambertian(Vec3(0.8f, 0.3f, 0.3f)));
	list[1] = new Sphere(Vec3(0.0f, -100.5f, -1.0f), 100.0f, new Lambertian(Vec3(0.2f, 0.4f, 0.0f)));
	list[2] = new Sphere(Vec3(1.0f, 0.0f, -1.0f), 0.5f, new Metal(Vec3(0.8f, 0.8f, 0.8f), 0.05f));
	list[3] = new Sphere(Vec3(-1.0f, 0.0f, -1.0f), 0.5f, new Metal(Vec3(0.8f, 0.6f, 0.2f), 0.8f));
	HitableList *world = new HitableList(list, 4);
	Camera camera;

	std::cout << "Generating output file..." << std::endl;
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
				col += Color(r, world, 0);
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
	std::cout << "File generation complete." << std::endl;
	return 0;
}

Vec3 Color(const Ray& r, HitableList *world, int depth) {
	HitRecord rec;
	if (world->Hit(r, 0.001f, std::numeric_limits<float>::max(), rec)) {
		Ray scatteredRay;
		Vec3 attenuation;
		if (depth < 50 && rec.materialPtr->Scatter(r, rec, attenuation, scatteredRay)) {
			return attenuation * Color(scatteredRay, world, depth + 1);
		} else {
			return Vec3(0.0f, 0.0f, 0.0f);
		}
	}
	else {
		Vec3 unitDirection = UnitVector(r.direction);
		const float t = 0.5f * (unitDirection.y() + 1.0f);
		// Linear Interpolation
		// blendedValue = (1-t)*startValue + t*endValue => where 0.0 < t < 1.0
		return (1.0f - t)*Vec3(1.0f, 1.0f, 1.0f) + t*Vec3(0.5f, 0.7f, 1.0f);
	}
}
