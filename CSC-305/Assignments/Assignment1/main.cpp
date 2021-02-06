#include "assignment.hpp"

#include <iostream>
#include <algorithm>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <random>
#include <chrono>

int main()
{
	const std::size_t HEIGHT{ 600 };
	const std::size_t WIDTH{ 600 };
	const std::size_t NUM_SAMPLES{ 16 };

	// Create the spheres
	TWCSphere sphere_1(math::Point(0.0f, 0.0f, 0.0f), 50.0f, Colour(1.0f, 0.5f, 0.0f));
	TWCSphere sphere_2(math::Point(-50.0f, 0.0f, -5.0f), 40.0f, Colour(1.0f, 1.0f, 0.0f));
	TWCSphere sphere_3(math::Point(50.0f, -20.0f, -5.0f), 30.0f, Colour(0.0f, 0.5f, 0.0f));
	TWCSphere sphere_4(math::Point(0.0f, -90.0f, -50.0f), 60.0f, Colour(0.0f, 0.5f, 1.0f));
	std::vector<TWCSphere> spheres{ sphere_1 , sphere_2 , sphere_3, sphere_4 };

	// Create the planes
	TWCPlane plane_1(math::Point(0.0f, 0.0f, -400.0f), math::Vector(0.0f, 0.0f, 1.0f), Colour(1.0f, 0.0f, 0.0f));
	TWCPlane plane_2(math::Point(0.0f, -80.0f, -205.0f), math::Vector(1.0f, -1.0f, 1.0f), Colour(0.8, 0.2, 0.3));
	std::vector<TWCPlane> planes{ plane_1, plane_2 };

	// Create the triangles
	TWCTriangle triangle_1(math::Point(-100.0f, 0.0f, -5.0f), math::Point(-350.0f, 0.0f, -5.0f), math::Point(-250.0f, 220.0f, -5.0f), Colour(1.0f, 1.0f, 1.0f));
	TWCTriangle triangle_2(math::Point(100.0f, 0.0f, -5.0f), math::Point(350.0f, 0.0f, -5.0f), math::Point(200.0f, -220.0f, -5.0f), Colour(0.0f, 1.0f, 1.0f));
	std::vector<TWCTriangle> triangles{ triangle_1 , triangle_2 };

	std::vector<Colour> img(HEIGHT * WIDTH); // This will be freed at the end
	// Create the camera plane
	for (size_t y = 0; y < HEIGHT; y++)
	{
		for (size_t x = 0; x < WIDTH; x++)
		{
			Colour pixel(0.0f, 0.0f, 0.0f);
			for (size_t s = 0; s < NUM_SAMPLES; s++)
			{
				// calc origin and direction
				math::Ray<math::Vector> r;
				// NOTE: (float)rand() / RAND_MAX is a range from [0.0f,1.0f]
				r.o = math::Vector((x - ((float)WIDTH - ((float)rand() / RAND_MAX)) / 2.0f), ((float)y - ((float)HEIGHT - ((float)rand() / RAND_MAX)) / 2.0f), (100)); // If we just vary the what we subtract HEIGHT and WIDTH by between [0, 1] we get random sampling
				r.d = math::Vector(0, 0, -1);

				// Calc the resulting color
				pixel += RaySceneIntersection(r, spheres, planes, triangles); // Multi sampling happens before we set the final pixel color
			}
			// Set the value
			img[x + y * WIDTH] = pixel / (float)NUM_SAMPLES; // we divide by NUM_SAMPLES to get the average of all the colours
		}
	}

	saveToBMP("testing_random.bmp", WIDTH, HEIGHT, img);

	// This time we use n rooks
	// Create the camera plane
	for (size_t y = 0; y < HEIGHT; y++)
	{
		for (size_t x = 0; x < WIDTH; x++)
		{
			Colour pixel(0.0f, 0.0f, 0.0f);
			std::vector<float> xs(NUM_SAMPLES);
			std::vector<float> ys(NUM_SAMPLES);
			for (size_t i = 0; i < NUM_SAMPLES; i++)
			{
				float m = (1.0f / NUM_SAMPLES) * (float)i;
				xs[i] = m;
				ys[i] = m;
			}

			// Randomize the xs and ys
			std::shuffle(xs.begin(), xs.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
			std::shuffle(ys.begin(), ys.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));

			// Now we run the code using the samples
			for (size_t s = 0; s < NUM_SAMPLES; s++)
			{
				// calc origin and direction
				math::Ray<math::Vector> r;
				// Use our randomized x's and y's
				r.o = math::Vector((x - ((float)WIDTH - xs[s]) / 2.0f), ((float)y - ((float)HEIGHT - ys[s]) / 2.0f), (100));
				r.d = math::Vector(0, 0, -1);

				// Calc the resulting color
				pixel += RaySceneIntersection(r, spheres, planes, triangles); // Multi sampling happens before we set the final pixel color
			}
			// Set the value
			img[x + y * WIDTH] = pixel / (float)NUM_SAMPLES; // we divide by NUM_SAMPLES to get the average of all the colours
		}
	}

	// For each pixel send out the ray
	saveToBMP("testing_rooks.bmp", WIDTH, HEIGHT, img);
	return 0;
}

/**
 * Saves a BMP image file based on the given array of pixels. All pixel values
 * have to be in the range [0, 1].
 *
 * @param filename The name of the file to save to.
 * @param width The width of the image.
 * @param height The height of the image.
 * @param image The array of pixels representing the image.
 */
void saveToBMP(std::string const& filename,
	std::size_t width,
	std::size_t height,
	std::vector<Colour> const& image)
{
	std::vector<unsigned char> data(image.size() * 3);

	for (std::size_t i{ 0 }, k{ 0 }; i < image.size(); ++i, k += 3)
	{
		Colour pixel = image[i];
		data[k + 0] = static_cast<unsigned char>(pixel.r * 255);
		data[k + 1] = static_cast<unsigned char>(pixel.g * 255);
		data[k + 2] = static_cast<unsigned char>(pixel.b * 255);
	}

	stbi_write_bmp(filename.c_str(),
		static_cast<int>(width),
		static_cast<int>(height),
		3,
		data.data());
}


std::optional<std::pair<float, Colour>> TWCSphere::GetIntersection(math::Ray<math::Vector> const& ray) {
	math::Vector origin_to_center = this->m_center - ray.o; // v vector

	float v_on_u = glm::dot(origin_to_center, ray.d); // This is the amount v is in the distance of u (the direction of the ray)
	if (v_on_u <= 0.0f) { // is the sphere behind the ray direction?
		return std::nullopt;
	}

	float d_squared = glm::dot(origin_to_center, origin_to_center) - (v_on_u * v_on_u); // This is the squared distance of the center of the sphere to the closest point on the ray to the sphere.
	if (d_squared > this->m_radius* this->m_radius) {
		return std::nullopt;
	}

	float dist_ray_to_center = sqrt(this->m_radius * this->m_radius - d_squared);
	float dist_from_origin = v_on_u - dist_ray_to_center;

	// if the val is negative its behind the camera so choose the further val
	if (dist_from_origin < 0.0f) {
		dist_from_origin = v_on_u + dist_ray_to_center;
	}

	return std::make_pair(dist_from_origin, this->m_colour);
}

Colour RaySceneIntersection(math::Ray<math::Vector> const& ray, std::vector<TWCSphere> const& spheres, std::vector<TWCPlane> const& planes, std::vector<TWCTriangle> triangles) {
	// Keeps track of the closest hit
	std::pair<float, Colour> closest_hit = std::make_pair(std::numeric_limits<float>::max(), Colour(0.0f, 0.0f, 0.0f)); // This colour is the background color

	// Do all intersections with the spheres
	for each (TWCSphere sphere in spheres)
	{
		auto result = sphere.GetIntersection(ray);
		if (result.has_value()) {
			auto r = result.value();
			if (r.first < closest_hit.first) {
				closest_hit = r;
			}
		}
	}

	// Do all intersections with the planes
	for each (TWCPlane plane in planes)
	{
		auto result = plane.GetIntersection(ray);
		if (result.has_value()) {
			auto r = result.value();
			if (r.first < closest_hit.first) {
				closest_hit = r;
			}
		}
	}

	// Do all intersections with the triangles
	for each (TWCTriangle triangle in triangles) {
		auto result = triangle.GetIntersection(ray);
		if (result.has_value()) {
			auto r = result.value();
			if (r.first < closest_hit.first) {
				closest_hit = r;
			}
		}
	}

	return closest_hit.second;
}

std::optional<std::pair<float, Colour>> TWCPlane::GetIntersection(math::Ray<math::Vector> const& ray) {
	float d = glm::dot(glm::normalize(ray.d), this->m_normal);
	if (d == 0.0f) {
		return std::nullopt;
	}
	float dot = glm::dot(this->m_point - ray.o, this->m_normal);
	float dist_from_origin = dot / d;
	if (dist_from_origin < 0.0f) {
		return std::nullopt;
	}

	return std::make_pair(dist_from_origin, this->m_colour);
}

std::optional<std::pair<float, Colour>> TWCTriangle::GetIntersection(math::Ray<math::Vector> const& ray) {
	math::Vector v1_v2 = this->m_v2 - this->m_v1;
	math::Vector v1_v3 = this->m_v3 - this->m_v1;
	math::Vector v2_v3 = this->m_v3 - this->m_v2;
	math::Vector v3_v1 = this->m_v1 - this->m_v3;
	math::Vector normal = glm::normalize(glm::cross(v1_v2, v1_v3));
	TWCPlane plane(this->m_v1, normal, this->m_colour);

	std::optional<std::pair<float, Colour>> result = plane.GetIntersection(ray);
	if (!result.has_value()) {
		return std::nullopt;
	}

	float t = result.value().first;

	math::Point point_of_possible_intersection = ray.d * t + ray.o;
	//std::cout << point_of_possible_intersection.x << " " << point_of_possible_intersection.y << " " << point_of_possible_intersection.z << std::endl;

	math::Vector v1_to_point_of_point_of_possible_intersection = point_of_possible_intersection - this->m_v1;
	if (glm::dot(normal, glm::cross(v1_v2, v1_to_point_of_point_of_possible_intersection)) < 0) {
		return std::nullopt;
	}

	math::Vector v2_to_point_of_point_of_possible_intersection = point_of_possible_intersection - this->m_v2;
	if (glm::dot(normal, glm::cross(v2_v3, v2_to_point_of_point_of_possible_intersection)) < 0) {
		return std::nullopt;
	}

	math::Vector v3_to_point_of_point_of_possible_intersection = point_of_possible_intersection - this->m_v3;
	if (glm::dot(normal, glm::cross(v3_v1, v3_to_point_of_point_of_possible_intersection)) < 0) {
		return std::nullopt;
	}

	//std::cout << "HIT\n";
	return std::make_pair(t, this->m_colour);
}