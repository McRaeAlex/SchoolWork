#pragma once

#include <atlas/math/Math.hpp>
#include <atlas/math/Ray.hpp>

#include <fmt/printf.h>
#include <stb_image.h>
#include <stb_image_write.h>

#include <vector>
#include <optional>
#include <utility>

using namespace atlas;
using Colour = math::Vector;

class TWCTriangle {
private:
	math::Point m_v1;
	math::Point m_v2;
	math::Point m_v3;
	Colour m_colour;

public:
	TWCTriangle(math::Point v1, math::Point v2, math::Point v3, Colour c) : m_v1(v1), m_v2(v2), m_v3(v3), m_colour(c)
	{};

	std::optional<std::pair<float, Colour>> GetIntersection(math::Ray<math::Vector> const& ray);
};

class TWCPlane {
private:
	math::Vector m_normal;
	math::Point m_point;
	Colour m_colour;

public:
	TWCPlane(math::Point p, math::Vector n, Colour c) : m_normal(glm::normalize(n)), m_point(p), m_colour(c)
	{};

	std::optional<std::pair<float, Colour>> GetIntersection(math::Ray<math::Vector> const& ray);
};

class TWCSphere {
private:
	math::Point m_center;
	float m_radius;
	Colour m_colour;

public:
	TWCSphere(math::Point p, float r, Colour c) : m_center(p), m_radius(r), m_colour(c)
	{};

	std::optional<std::pair<float, Colour>> GetIntersection(math::Ray<math::Vector> const& ray);
};

Colour RaySceneIntersection(math::Ray<math::Vector> const& ray, std::vector<TWCSphere> const& spheres, std::vector<TWCPlane> const& planes, std::vector<TWCTriangle> triangles);

void saveToBMP(std::string const& filename,
	std::size_t width,
	std::size_t height,
	std::vector<Colour> const& image);

// Your code here.
