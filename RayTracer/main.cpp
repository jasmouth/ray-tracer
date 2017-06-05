#include <fstream>
#include <chrono>
#include <random>
#include <limits>
#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"
#include "Material.h"

Vec3 Color(const Ray& r, Hitable *world, int depth);

Hitable* CreateRandomScene(std::mt19937 rng, std::uniform_real_distribution<float> dist);

int main() {
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	std::mt19937 generator(seed);
	std::uniform_real_distribution<float> distribution(0.0f, 1.0f);

	int numX = 1200;
	int numY = 800;
	int numSamples = 100;

	Hitable *world = CreateRandomScene(generator, distribution);
	const Vec3 lookFrom = Vec3(13.0f, 2.0f, 3.0f);
	const Vec3 lookAt = Vec3(0.0f, 0.0f, 0.0f);
	const float focusDistance = 10.0f;
	const float aperture = 0.1f;
	Camera camera(
		lookFrom,
		lookAt,
		Vec3(0.0f, 1.0f, 0.0f),
		20.0f,
		float(numX) / float(numY),
		aperture,
		focusDistance
	);

	std::cout << "Generating output file..." << std::endl;
	std::ofstream outputFile;
	outputFile.open("output.ppm");
	outputFile << "P3\n" << numX << " " << numY << "\n255\n";
	for (int y = numY - 1; y >= 0; y--) {
		for (int x = 0; x < numX; x++) {
			Vec3 col(0.0f, 0.0f, 0.0f);
			for (int k = 0; k < numSamples; k++) {
				const float s = float(x + distribution(generator)) / float(numX);
				const float t = float(y + distribution(generator)) / float(numY);
				Ray r = camera.CreateRay(s, t);
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

Vec3 Color(const Ray& r, Hitable *world, int depth) {
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

Hitable* CreateRandomScene(std::mt19937 rng, std::uniform_real_distribution<float> dist) {
	int i = 0;
	Hitable **list = new Hitable*[500];
	list[i++] = new Sphere(Vec3(0.0f, -1000.0f, 0.0f), 1000.0f, new Lambertian(Vec3(0.5f, 0.5f, 0.5f)));
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			const float materialChoice = dist(rng);
			const Vec3 center = Vec3(a + 0.9f * dist(rng), 0.2f, b + 0.9f * dist(rng));
			if ((center - Vec3(4.0f, 0.2f, 0.0f)).Length() > 0.9f) {
				Material *material;
				if (materialChoice < 0.8f) { // Diffuse
					material = new Lambertian(
						Vec3(
							dist(rng) * dist(rng),
							dist(rng) * dist(rng),
							dist(rng) * dist(rng)
						)
					);
				} else if (materialChoice < 0.95f) { // Metal
					material = new Metal(
						Vec3(
							0.5f * (1 + dist(rng)),
							0.5f * (1 + dist(rng)),
							0.5f * (1 + dist(rng))
						), 0.5f * dist(rng)
					);
				} else { // Glass
					material = new Dielectric(1.5f);
				}
				list[i++] = new Sphere(center, 0.2f, material);
			}
		}
	}

	list[i++] = new Sphere(Vec3(0.0f, 1.0f, 0.0f), 1.0f, new Dielectric(1.5f));
	list[i++] = new Sphere(Vec3(-4.0f, 1.0f, 0.0f), 1.0f, new Lambertian(Vec3(0.4f, 0.2f, 0.1f)));
	list[i++] = new Sphere(Vec3(4.0f, 1.0f, 0.0f), 1.0f, new Metal(Vec3(0.7f, 0.6f, 0.5f), 0.0f));
	
	return new HitableList(list, i);
}
