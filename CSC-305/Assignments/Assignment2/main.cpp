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
	// Setup the world
	std::shared_ptr<World> world{ std::make_shared<World>() };
	world->width = 600;
	world->height = 600;
	world->background = { 0,0,0 };
	world->sampler = std::make_shared<NRooks>(16, 83);

	// Add objects
	world->scene.push_back(
		std::make_shared<Sphere>(atlas::math::Point{ 0, 0, -600 }, 128.0f));
	world->scene[0]->setMaterial(
		std::make_shared<Matte>(0.50f, 0.05f, Colour{ 1, 0, 0 }));
	world->scene[0]->setColour({ 1, 0, 0 });

	world->scene.push_back(
		std::make_shared<Sphere>(atlas::math::Point{ 128, 32, -700 }, 64.0f));
	world->scene[1]->setMaterial(
		std::make_shared<Matte>(0.50f, 0.05f, Colour{ 0, 0, 1 }));
	world->scene[1]->setColour({ 0, 0, 1 });

	world->scene.push_back(
		std::make_shared<Triangle>(atlas::math::Point{ 130, 32, -600 }, atlas::math::Point{ 160, 200, -600 }, atlas::math::Point{ 300, 32, -600 }));
	world->scene[2]->setMaterial(
		std::make_shared<Matte>(0.50f, 0.05f, Colour{ 0, 1, 0 }));
	world->scene[2]->setColour({ 0, 1, 0 });

	// Add ground plane
	world->scene.push_back(
		std::make_shared<Plane>(atlas::math::Point{ 0,200, 0 }, atlas::math::Vector{ 0, -1, 0 }));
	world->scene[3]->setMaterial(
		std::make_shared<Matte>(0.50f, 0.05f, Colour{ 1, 1, 1 }));
	world->scene[3]->setColour({ 1, 1, 0 });

	// Add lights

	// Add ambient
	world->ambient = std::make_shared<Ambient>();
	world->ambient->setColour({ 1, 1, 1 });
	world->ambient->scaleRadiance(0.4f);

	// Add directional
	world->lights.push_back(
		std::make_shared<Directional>(atlas::math::Vector{ 0, 0, 10 }));
	world->lights[0]->setColour({ 1, 1, 1 });
	world->lights[0]->scaleRadiance(4.0f);

	// Add point light
	world->lights.push_back(
		std::make_shared<Point>(atlas::math::Point{ -300,0,-200 }));
	world->lights[1]->setColour({ 1, 1, 1 });
	world->lights[1]->scaleRadiance(0.1f);

	// Render
	Pinhole pinhole_camera{};
	pinhole_camera.renderScene(world);

	float max{ 1.0f };
	for (auto& i : world->image) {
		float temp = std::max(std::max(i.r, i.g), i.b);
		if (temp > max) {
			max = temp;
		}
	}

	for (std::size_t i{ 0 }; i < world->image.size(); ++i) {
		world->image[i] = world->image[i] / max;
	}

	saveToBMP("pinholecamera_nrooks_correct.bmp", world->width, world->height, world->image);

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

// ******* Function Member Implementation *******

// ***** Shape function members *****
Shape::Shape() : mColour{ 0, 0, 0 }
{}

void Shape::setColour(Colour const& col)
{
	mColour = col;
}

Colour Shape::getColour() const
{
	return mColour;
}

void Shape::setMaterial(std::shared_ptr<Material> const& material)
{
	mMaterial = material;
}

std::shared_ptr<Material> Shape::getMaterial() const
{
	return mMaterial;
}

// ***** Sampler function members *****
Sampler::Sampler(int numSamples, int numSets) :
	mNumSamples{ numSamples }, mNumSets{ numSets }, mCount{ 0 }, mJump{ 0 }
{
	mSamples.reserve(mNumSets* mNumSamples);
	setupShuffledIndeces();
}

int Sampler::getNumSamples() const
{
	return mNumSamples;
}

void Sampler::setupShuffledIndeces()
{
	mShuffledIndeces.reserve(mNumSamples * mNumSets);
	std::vector<int> indices;

	std::random_device d;
	std::mt19937 generator(d());

	for (int j = 0; j < mNumSamples; ++j)
	{
		indices.push_back(j);
	}

	for (int p = 0; p < mNumSets; ++p)
	{
		std::shuffle(indices.begin(), indices.end(), generator);

		for (int j = 0; j < mNumSamples; ++j)
		{
			mShuffledIndeces.push_back(indices[j]);
		}
	}
}

atlas::math::Point Sampler::sampleUnitSquare()
{
	if (mCount % mNumSamples == 0)
	{
		atlas::math::Random<int> engine;
		mJump = (engine.getRandomMax() % mNumSets) * mNumSamples;
	}

	return mSamples[mJump + mShuffledIndeces[mJump + mCount++ % mNumSamples]];
}

// ***** Light function members *****
Colour Light::L([[maybe_unused]] ShadeRec& sr)
{
	return mRadiance * mColour;
}

void Light::scaleRadiance(float b)
{
	mRadiance = b;
}

void Light::setColour(Colour const& c)
{
	mColour = c;
}

// ***** Sphere function members *****
Sphere::Sphere(atlas::math::Point center, float radius) :
	mCentre{ center }, mRadius{ radius }, mRadiusSqr{ radius * radius }
{}

bool Sphere::hit(atlas::math::Ray<atlas::math::Vector> const& ray,
	ShadeRec& sr) const
{
	atlas::math::Vector tmp = ray.o - mCentre;
	float t{ std::numeric_limits<float>::max() };
	bool intersect{ intersectRay(ray, t) };

	// update ShadeRec info about new closest hit
	if (intersect && t < sr.t)
	{
		sr.normal = (tmp + t * ray.d) / mRadius;
		sr.ray = ray;
		sr.color = mColour;
		sr.t = t;
		sr.material = mMaterial;
	}

	return intersect;
}

bool Sphere::intersectRay(atlas::math::Ray<atlas::math::Vector> const& ray,
	float& tMin) const
{
	const auto tmp{ ray.o - mCentre };
	const auto a{ glm::dot(ray.d, ray.d) };
	const auto b{ 2.0f * glm::dot(ray.d, tmp) };
	const auto c{ glm::dot(tmp, tmp) - mRadiusSqr };
	const auto disc{ (b * b) - (4.0f * a * c) };

	if (atlas::core::geq(disc, 0.0f))
	{
		const float kEpsilon{ 0.01f };
		const float e{ std::sqrt(disc) };
		const float denom{ 2.0f * a };

		// Look at the negative root first
		float t = (-b - e) / denom;
		if (atlas::core::geq(t, kEpsilon))
		{
			tMin = t;
			return true;
		}

		// Now the positive root
		t = (-b + e);
		if (atlas::core::geq(t, kEpsilon))
		{
			tMin = t;
			return true;
		}
	}

	return false;
}

// ***** Regular function members *****
Regular::Regular(int numSamples, int numSets) : Sampler{ numSamples, numSets }
{
	generateSamples();
}

void Regular::generateSamples()
{
	int n = static_cast<int>(glm::sqrt(static_cast<float>(mNumSamples)));

	for (int j = 0; j < mNumSets; ++j)
	{
		for (int p = 0; p < n; ++p)
		{
			for (int q = 0; q < n; ++q)
			{
				mSamples.push_back(
					atlas::math::Point{ (q + 0.5f) / n, (p + 0.5f) / n, 0.0f });
			}
		}
	}
}

// ***** Regular function members *****
Random::Random(int numSamples, int numSets) : Sampler{ numSamples, numSets }
{
	generateSamples();
}

void Random::generateSamples()
{
	atlas::math::Random<float> engine;
	for (int p = 0; p < mNumSets; ++p)
	{
		for (int q = 0; q < mNumSamples; ++q)
		{
			mSamples.push_back(atlas::math::Point{
				engine.getRandomOne(), engine.getRandomOne(), 0.0f });
		}
	}
}

// ***** Lambertian function members *****
Lambertian::Lambertian() : mDiffuseColour{}, mDiffuseReflection{}
{}

Lambertian::Lambertian(Colour diffuseColor, float diffuseReflection) :
	mDiffuseColour{ diffuseColor }, mDiffuseReflection{ diffuseReflection }
{}

Colour
Lambertian::fn([[maybe_unused]] ShadeRec const& sr,
	[[maybe_unused]] atlas::math::Vector const& reflected,
	[[maybe_unused]] atlas::math::Vector const& incoming) const
{
	return mDiffuseColour * mDiffuseReflection * glm::one_over_pi<float>();
}

Colour
Lambertian::rho([[maybe_unused]] ShadeRec const& sr,
	[[maybe_unused]] atlas::math::Vector const& reflected) const
{
	return mDiffuseColour * mDiffuseReflection;
}

void Lambertian::setDiffuseReflection(float kd)
{
	mDiffuseReflection = kd;
}

void Lambertian::setDiffuseColour(Colour const& colour)
{
	mDiffuseColour = colour;
}

// ***** Matte function members *****
Matte::Matte() :
	Material{},
	mDiffuseBRDF{ std::make_shared<Lambertian>() },
	mAmbientBRDF{ std::make_shared<Lambertian>() }
{}

Matte::Matte(float kd, float ka, Colour color) : Matte{}
{
	setDiffuseReflection(kd);
	setAmbientReflection(ka);
	setDiffuseColour(color);
}

void Matte::setDiffuseReflection(float k)
{
	mDiffuseBRDF->setDiffuseReflection(k);
}

void Matte::setAmbientReflection(float k)
{
	mAmbientBRDF->setDiffuseReflection(k);
}

void Matte::setDiffuseColour(Colour colour)
{
	mDiffuseBRDF->setDiffuseColour(colour);
	mAmbientBRDF->setDiffuseColour(colour);
}

Colour Matte::shade(ShadeRec& sr)
{
	using atlas::math::Ray;
	using atlas::math::Vector;

	Vector wo = -sr.ray.o;
	Colour L = mAmbientBRDF->rho(sr, wo) * sr.world->ambient->L(sr);
	size_t numLights = sr.world->lights.size();

	for (size_t i{ 0 }; i < numLights; ++i)
	{
		Vector wi = sr.world->lights[i]->getDirection(sr);
		float nDotWi = glm::dot(sr.normal, wi);

		if (nDotWi > 0.0f)
		{
			L += mDiffuseBRDF->fn(sr, wo, wi) * sr.world->lights[i]->L(sr) *
				nDotWi;
		}
	}

	return L;
}

// ***** Directional function members *****
Directional::Directional() : Light{}
{}

Directional::Directional(atlas::math::Vector const& d) : Light{}
{
	setDirection(d);
}

void Directional::setDirection(atlas::math::Vector const& d)
{
	mDirection = glm::normalize(d);
}

atlas::math::Vector Directional::getDirection([[maybe_unused]] ShadeRec& sr)
{
	return mDirection;
}

// ***** Ambient function members *****
Ambient::Ambient() : Light{}
{}

atlas::math::Vector Ambient::getDirection([[maybe_unused]] ShadeRec& sr)
{
	return atlas::math::Vector{ 0.0f };
}

Camera::Camera() :
	mEye{ 0.0f, 0.0f, 500.0f },
	mLookAt{ 0.0f },
	mUp{ 0.0f, 1.0f, 0.0f },
	mU{ 1.0f, 0.0f, 0.0f },
	mV{ 0.0f, 1.0f, 0.0f },
	mW{ 0.0f, 0.0f, 1.0f }
{}

void Camera::setEye(atlas::math::Point const& eye)
{
	mEye = eye;
}

void Camera::setLookAt(atlas::math::Point const& lookAt)
{
	mLookAt = lookAt;
}

void Camera::setUpVector(atlas::math::Vector const& up)
{
	mUp = up;
}

void Camera::computeUVW()
{
	mW = glm::normalize(mEye - mLookAt);
	mU = glm::normalize(glm::cross(mUp, mW));
	mV = glm::cross(mW, mU);

	if (areEqual(mEye.x, mLookAt.x) && areEqual(mEye.z, mLookAt.z) &&
		mEye.y > mLookAt.y)
	{
		mU = { 0.0f, 0.0f, 1.0f };
		mV = { 1.0f, 0.0f, 0.0f };
		mW = { 0.0f, 1.0f, 0.0f };
	}

	if (areEqual(mEye.x, mLookAt.x) && areEqual(mEye.z, mLookAt.z) &&
		mEye.y < mLookAt.y)
	{
		mU = { 1.0f, 0.0f, 0.0f };
		mV = { 0.0f, 0.0f, 1.0f };
		mW = { 0.0f, -1.0f, 0.0f };
	}
}

// Pinhole Camera

Pinhole::Pinhole() : Camera{}, mDistance{ 500.0f }, mZoom{ 1.0f }
{}

void Pinhole::SetDistance(float dist)
{
	this->mDistance = dist;
}

void Pinhole::setZoom(float zoom)
{
	this->mZoom = zoom;
}

atlas::math::Vector Pinhole::rayDirection(atlas::math::Point const& p) const
{
	const atlas::math::Vector dir = p.x * mU + p.y * mV - mDistance * mW;
	return glm::normalize(dir);
}

void Pinhole::renderScene(std::shared_ptr<World>& world) const
{
	using atlas::math::Point;
	using atlas::math::Ray;
	using atlas::math::Vector;

	Point samplePoint{}, pixelPoint{};
	Ray<atlas::math::Vector> ray{};

	ray.o = mEye;
	float avg{ 1.0f / world->sampler->getNumSamples() };

	for (int r{ 0 }; r < world->height; ++r)
	{
		for (int c{ 0 }; c < world->width; ++c)
		{
			Colour pixelAverage{ 0, 0, 0 };

			for (int j = 0; j < world->sampler->getNumSamples(); ++j)
			{
				ShadeRec trace_data{};
				trace_data.world = world;
				trace_data.t = std::numeric_limits<float>::max();
				samplePoint = world->sampler->sampleUnitSquare();
				pixelPoint.x = c - 0.5f * world->width + samplePoint.x;
				pixelPoint.y = r - 0.5f * world->height + samplePoint.y;
				ray.d = rayDirection(pixelPoint);
				bool hit{};

				for (auto const& obj : world->scene)
				{
					hit |= obj->hit(ray, trace_data);
				}

				if (hit) {
					pixelAverage += trace_data.material->shade(trace_data);
				}
			}

			world->image.push_back({ pixelAverage.r * avg,
								   pixelAverage.g * avg,
								   pixelAverage.b * avg });
		}
	}
}



Triangle::Triangle(atlas::math::Point v1, atlas::math::Point v2, atlas::math::Point v3) : mV1{ v1 }, mV2{ v2 }, mV3{ v3 }
{
	atlas::math::Vector v1_to_v2{ glm::normalize(mV2 - mV1) };
	atlas::math::Vector v1_to_v3{ glm::normalize(mV3 - mV1) };
	mNormal = glm::normalize(glm::cross(v1_to_v2, v1_to_v3));
}

bool Triangle::hit(atlas::math::Ray<atlas::math::Vector> const& ray, ShadeRec& sr) const
{
	float t{ std::numeric_limits<float>::max() };
	bool intersect{ intersectRay(ray, t) };

	// update ShadeRec info about new closest hit
	if (intersect && t < sr.t)
	{
		sr.normal = -mNormal;
		sr.ray = ray;
		sr.color = mColour;
		sr.t = t;
		sr.material = mMaterial;
	}

	return intersect;
}

bool Triangle::intersectRay(atlas::math::Ray<atlas::math::Vector> const& ray, float& tMin) const
{
	// Create the vectors
	atlas::math::Vector v1_v2 = this->mV2 - this->mV1;
	atlas::math::Vector v1_v3 = this->mV3 - this->mV1;
	atlas::math::Vector v2_v3 = this->mV3 - this->mV2;
	atlas::math::Vector v3_v1 = this->mV1 - this->mV3;

	// Check to see if the ray is perp to the normal
	float d = glm::dot(ray.d, mNormal);
	if (d == 0.0f) {
		return false;
	}
	// Calculate where it hits
	float dot = glm::dot(this->mV1 - ray.o, this->mNormal);
	float dist_from_origin = dot / d;
	if (dist_from_origin < 0.0f) {
		return false;
	}

	atlas::math::Point point_of_possible_intersection = ray.d * dist_from_origin + ray.o;
	//std::cout << point_of_possible_intersection.x << " " << point_of_possible_intersection.y << " " << point_of_possible_intersection.z << std::endl;

	atlas::math::Vector v1_to_point_of_point_of_possible_intersection = point_of_possible_intersection - this->mV1;
	if (glm::dot(mNormal, glm::cross(v1_v2, v1_to_point_of_point_of_possible_intersection)) < 0) {
		return false;
	}

	atlas::math::Vector v2_to_point_of_point_of_possible_intersection = point_of_possible_intersection - this->mV2;
	if (glm::dot(mNormal, glm::cross(v2_v3, v2_to_point_of_point_of_possible_intersection)) < 0) {
		return false;
	}

	atlas::math::Vector v3_to_point_of_point_of_possible_intersection = point_of_possible_intersection - this->mV3;
	if (glm::dot(mNormal, glm::cross(v3_v1, v3_to_point_of_point_of_possible_intersection)) < 0) {
		return false;
	}

	tMin = dist_from_origin;
	return true;
}

Plane::Plane(atlas::math::Point p, atlas::math::Vector n) : mPoint{ p }, mNormal{ glm::normalize(n) }
{
}

bool Plane::hit(atlas::math::Ray<atlas::math::Vector> const& ray, ShadeRec& sr) const
{
	float t{ std::numeric_limits<float>::max() };
	bool intersect{ intersectRay(ray, t) };

	// update ShadeRec info about new closest hit
	if (intersect && t < sr.t)
	{
		sr.normal = mNormal;
		sr.ray = ray;
		sr.color = mColour;
		sr.t = t;
		sr.material = mMaterial;
	}

	return intersect;
}

bool Plane::intersectRay(atlas::math::Ray<atlas::math::Vector> const& ray, float& tMin) const
{
	float d = glm::dot(glm::normalize(ray.d), this->mNormal);
	if (d == 0.0f) {
		return false;
	}
	float dot = glm::dot(this->mPoint - ray.o, this->mNormal);
	float dist_from_origin = dot / d;
	if (dist_from_origin < 0.0f) {
		return false;
	}

	tMin = dist_from_origin;
	return true;
}

Point::Point(atlas::math::Point p) : mPoint{ p }, Light{} {}

atlas::math::Vector Point::getDirection(ShadeRec& sr)
{
	atlas::math::Vector point = sr.ray.d * sr.t + sr.ray.o;
	return mPoint - point;
}

Colour Point::L([[maybe_unused]] ShadeRec& sr) {
	return mRadiance * mColour;
}

NRooks::NRooks(int numSamples, int numSets) : Sampler{ numSamples, numSets } {
	generateSamples();
}

void NRooks::generateSamples() {
	std::vector<float> xs{ };
	std::vector<float> ys{ };

	for (int i = 0; i < mNumSamples; ++i) {
		float div = (float)i / (float)mNumSamples;
		xs.push_back(div);
		ys.push_back(div);
	}

	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	unsigned seed2 = std::chrono::system_clock::now().time_since_epoch().count();

	for (int p = 0; p < mNumSets; ++p)
	{
		// Shuffle the arrays randomly then place the samples in the vector to create sets of nRooks
		std::shuffle(xs.begin(), xs.end(), std::default_random_engine(seed));
		std::shuffle(ys.begin(), ys.end(), std::default_random_engine(seed2));

		for (int q = 0; q < mNumSamples; ++q)
		{
			mSamples.push_back(atlas::math::Point{ xs[q], ys[q], 0.0f });
		}
	}
}