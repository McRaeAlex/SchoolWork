#include "assignment.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include <iostream>
#include <algorithm>
#include <limits>
#include <cmath>
#include <cstdlib>
#include <random>
#include <chrono>
#include <thread>

#include <atlas/core/Timer.hpp>


int main()
{
#pragma region CreateWorld
	std::shared_ptr<World> world{ std::make_shared<World>() };
	world->height = 600;
	world->width = 600;
	world->background = { 0,0,0 };
	world->sampler = std::make_shared<NRooksSampler>(16, 83);
#pragma endregion

#pragma region CreateMaterials
	std::shared_ptr<Matte> red{ std::make_shared<Matte>(1.0f, 1.0f, Colour{1.0f, 0.0f, 0.0f}) };
	std::shared_ptr<Matte> blue_green{ std::make_shared<Matte>(1.0f, 1.0f, Colour{0.25f, 0.9f, 0.8f}) }; // to do make this reflect a bit like water



	std::shared_ptr<Phong> glossy_blue_green{ std::make_shared<Phong>(1.0f, 0.2f, 1.0f, Colour{0.3f, 0.8f, 0.8f}, Colour{1.0f, 1.0f, 1.0f}) };
	std::shared_ptr<Phong> glossy_red{ std::make_shared<Phong>(0.4f, 0.1f, 0.5f, Colour{1.0f, 0.0f, 0.2f}, Colour{1.0f, 1.0f, 1.0f}) };
	std::shared_ptr<Phong> white{ std::make_shared<Phong>(1.0f, 1.0f, 1.0f, Colour{1.0f, 1.0f, 1.0f}, Colour{1.0f, 1.0f, 1.0f}) };
	std::shared_ptr<Phong> glossy_yellow{ std::make_shared<Phong>(0.45f, 0.1f, 0.5f, Colour{0.8f, 0.8f, 0.2f}, Colour{1.0f, 1.0f, 1.0f}) };

	Phong glossy_blue{ 0.4f, 0.1f, 0.5f, Colour{0.0f, 0.0f, 1.0f}, Colour{1.0f, 1.0f, 1.0f} };

	std::shared_ptr<SimpleTransparent> transparent_blue{ std::make_shared<SimpleTransparent>(glossy_blue, -0.99f) };


	std::shared_ptr<GlossyReflection> mirror_blue{ std::make_shared<GlossyReflection>(glossy_blue) };


#pragma endregion


#pragma region AddObjects
	std::shared_ptr<Triangle> t_1{ std::make_shared<Triangle>(math::Point(-200, -100, 40), math::Point(-110, -90, 40), math::Point(-150, 25, 100)) };
	t_1->setMaterial(mirror_blue);

	std::shared_ptr<Sphere> s_1{ std::make_shared<Sphere>(math::Point(100.0f, -20.0f, 200.0f), 50.0f) };
	s_1->setMaterial(glossy_red);

	std::shared_ptr<Sphere> s_2{ std::make_shared<Sphere>(math::Point(100.0f, -20.0f, 30.0f), 40.0f) };
	s_2->setMaterial(transparent_blue);

	std::shared_ptr<Sphere> s_3{ std::make_shared<Sphere>(math::Point(-170.0f, -20.0f, 20.0f), 35.0f) };
	s_3->setMaterial(std::make_shared<Phong>(glossy_blue));

	std::shared_ptr<Plane> ground{ std::make_shared<Plane>(math::Point(0, 300, 100.0f), math::Vector(0, -1, 0)) };
	ground->setMaterial(white);

	std::shared_ptr<Mesh> monkey{ std::make_shared<Mesh>("suzanne.obj", math::Point{300.0f, 50.0f, 200.0f}, -100.0f) };
	monkey->setMaterial(glossy_yellow);

	world->scene.push_back(ground);
	world->scene.push_back(s_1);
	world->scene.push_back(s_2);
	world->scene.push_back(s_3);
	world->scene.push_back(t_1);
	world->scene.push_back(monkey);
#pragma endregion

#pragma region AddLights

	// Add ambient
	world->ambient = std::make_shared<Ambient>();
	world->ambient->setColour({ 1, 1, 1 });
	world->ambient->scaleRadiance(0.5f);

	std::shared_ptr<PointLight> l_1{ std::make_shared<PointLight>(math::Point(100, -100, 0)) };
	l_1->setColour({ 1,1,1 });
	l_1->scaleRadiance(1.0f);

	std::shared_ptr<Directional> d_1{ std::make_shared<Directional>(math::Vector{0,-1,0}) };
	d_1->setColour({ 1,1,1 });
	d_1->scaleRadiance(1.0f);

	world->lights.push_back(l_1);
	world->lights.push_back(d_1);

#pragma endregion

#pragma region SetupCamera
	// Render
	float x{ 500 };
	PinHoleCamera camera{ x };
	camera.setEye(math::Point(0.0f, 0.0f, -x));
	camera.setLookAt(math::Point(0.0f, 0.0f, 0.0f));
	camera.computeUVW();

#pragma endregion

#pragma region Render
	auto t1 = std::chrono::high_resolution_clock::now();
	camera.renderScene(world);
	auto t2 = std::chrono::high_resolution_clock::now();

	std::cout << "f() took "
		<< std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count()
		<< " milliseconds\n";

	saveToBMP("Render.bmp", world->width, world->height, world->image);
#pragma endregion
	return 0;
}

#pragma region SaveToBMP

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

#pragma endregion

#pragma region World
Colour traceRay(const std::shared_ptr<World>& world, math::Ray<math::Vector> ray) {
	ShadeRec trace_data{};
	trace_data.world = world;
	trace_data.t = std::numeric_limits<float>::max();

	bool hit{ false };

	for (auto const& obj : world->scene)
	{
		hit |= obj->hit(ray, trace_data);
	}

	if (hit) {
		return trace_data.material->shade(trace_data);
	}

	return world->background;
}
#pragma endregion


#pragma region Samplers

Sampler::Sampler(int numSamples, int numSets) : mNumSamples{ numSamples }, mNumSets{ numSets }, mCount{ 0 }, mJump{ 0 }
{
	mSamples.reserve(mNumSets* (size_t)mNumSamples);
	setupShuffledIndeces();
}

int Sampler::getNumSamples() const
{
	return mNumSamples;
}

void Sampler::setupShuffledIndeces()
{
	mShuffledIndeces.reserve(mNumSamples * (size_t)mNumSets);
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

	return mSamples[(size_t)mJump + mShuffledIndeces[(size_t)mJump + mCount++ % mNumSamples]];
}

NRooksSampler::NRooksSampler(int numSamples, int numSets) : Sampler{ numSamples, numSets }
{
	generateSamples();
}

void NRooksSampler::generateSamples() {
	std::vector<float> xs{ };
	std::vector<float> ys{ };

	for (int i = 0; i < mNumSamples; ++i) {
		float div = (float)i / (float)mNumSamples;
		xs.push_back(div);
		ys.push_back(div);
	}

	unsigned int seed = std::chrono::system_clock::now().time_since_epoch().count();
	unsigned int seed2 = std::chrono::system_clock::now().time_since_epoch().count();

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

#pragma endregion

#pragma region BRDFs

#pragma region Lambertian

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

#pragma endregion

#pragma region Specular
Specular::Specular() : mSpecularColour{}, mSpecularReflection{}
{}

Specular::Specular(float intensity, Colour color) : mSpecularColour{ color }, mSpecularReflection{ intensity }
{}

Colour Specular::fn(ShadeRec const& sr,
	atlas::math::Vector const& reflected,
	atlas::math::Vector const& incoming) const {
	Colour L{ 0,0,0 };

	float n_dot_in = glm::dot(glm::normalize(sr.normal), glm::normalize(incoming));
	math::Vector r{ -incoming + 2.0f * sr.normal * n_dot_in };

	float r_dot_out = glm::dot(glm::normalize(r), glm::normalize(reflected));

	if (r_dot_out > 0.0f) {
		L = mSpecularColour * mSpecularReflection * powf(r_dot_out, 32.0f);
	}

	return L;
}

Colour Specular::rho(ShadeRec const& sr,
	atlas::math::Vector const& reflected) const {
	return { 0,0,0 };
};
void Specular::setDiffuseColour(Colour color) {
	mSpecularColour = color;
}

void Specular::setSpecularReflection(float ks) {
	mSpecularReflection = ks;
}
#pragma endregion

#pragma region GlossySpecular

GlossySpecular::GlossySpecular() {
}

GlossySpecular::~GlossySpecular() {
}

Colour GlossySpecular::fn(ShadeRec const& sr,
	atlas::math::Vector const& reflected,
	atlas::math::Vector const& incoming) const {
	// Calc the reflected ray direction out. trace the ray out into teh scene
	// return that color plus some modifications


	Colour L{ 0,0,0 };

	math::Ray<math::Vector> outgoing{};
	outgoing.d = incoming;

	math::Point hit = sr.ray.o + sr.t * sr.ray.d;
	outgoing.o = glm::dot(incoming, glm::normalize(sr.normal)) < 0.0f ? hit - (1e-3f * sr.normal) : hit + (1e-3f * sr.normal);

	return traceRay(sr.world, outgoing);
}

Colour GlossySpecular::rho(ShadeRec const& sr,
	atlas::math::Vector const& reflected) const {
	return { 0,0,0 };
}


#pragma endregion

#pragma region SimpleTransparent

// Basically we keep shooting the ray from where we got hit in the refracted direction and repreat until we hit something solid or nothing

#pragma endregion

#pragma endregion

#pragma region Materials

#pragma region Matte

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

	Vector wo = -sr.ray.d;
	Colour L = mAmbientBRDF->rho(sr, wo) * sr.world->ambient->L(sr);
	size_t numLights = sr.world->lights.size();

	math::Point location_hit = sr.t * sr.ray.d + sr.ray.o + (0.01f * sr.normal);

	for (size_t i{ 0 }; i < numLights; ++i)
	{
		Vector wi = sr.world->lights[i]->getDirection(sr);
		float nDotWi = glm::dot(sr.normal, wi);

		Ray<math::Vector> ray{};
		ray.o = location_hit;
		ray.d = wi;
		ShadeRec w{};
		bool hit{ false };

		for (auto const& obj : sr.world->scene)
		{
			hit |= obj->hit(ray, w);
		}

		if (nDotWi > 0.0f && !hit)
		{
			L += mDiffuseBRDF->fn(sr, wo, wi) * sr.world->lights[i]->L(sr) * nDotWi;
		}
	}

	return L;
}

#pragma endregion

#pragma region Phong

Phong::Phong(float kd, float ka, float ks, Colour color, Colour specularColor) : Material{}, mAmbientBRDF{ std::make_shared<Lambertian>() }, mDiffuseBRDF{ std::make_shared<Lambertian>() }, mSpecularBRDF{ std::make_shared<Specular>() }
{
	mDiffuseBRDF->setDiffuseReflection(kd);
	mAmbientBRDF->setDiffuseReflection(ka);
	mSpecularBRDF->setSpecularReflection(ks);


	mDiffuseBRDF->setDiffuseColour(color);
	mAmbientBRDF->setDiffuseColour(color);
	mSpecularBRDF->setDiffuseColour(specularColor);
}

Phong::Phong(const Phong& copy) : Material{}, mAmbientBRDF{ copy.mAmbientBRDF }, mDiffuseBRDF{ copy.mDiffuseBRDF }, mSpecularBRDF{ copy.mSpecularBRDF }
{
}

Colour Phong::shade(ShadeRec& sr)
{
	using atlas::math::Ray;
	using atlas::math::Vector;

	Vector wo = -sr.ray.d;
	Colour L = mAmbientBRDF->rho(sr, wo) * sr.world->ambient->L(sr);
	size_t numLights = sr.world->lights.size();

	math::Point location_hit = sr.t * sr.ray.d + sr.ray.o + (0.01f * sr.normal);

	for (size_t i{ 0 }; i < numLights; ++i)
	{
		Vector wi = sr.world->lights[i]->getDirection(sr);
		float nDotWi = glm::dot(sr.normal, wi);

		Ray<math::Vector> ray{};
		ray.o = location_hit;
		ray.d = wi;
		ShadeRec w{};
		bool hit{ false };

		for (auto const& obj : sr.world->scene)
		{
			hit |= obj->hit(ray, w);
		}

		if (nDotWi > 0.0f && !hit)
		{
			L += (mDiffuseBRDF->fn(sr, wo, wi) + mSpecularBRDF->fn(sr, wo, wi)) * sr.world->lights[i]->L(sr) * nDotWi;
		}
	}

	return L;
}

#pragma endregion

#pragma region SimpleTransparent

SimpleTransparent::SimpleTransparent(Phong base, float ref) : Phong{ base }, mRefractiveIndex{ ref }
{
}

Colour SimpleTransparent::shade(ShadeRec& sr)
{
	Colour L{ Phong::shade(sr) };

	math::Vector wo = -sr.ray.d;
	math::Vector wi{};

	// Calc the outgoing ray.
	float cosi = std::max(-1.0f, std::min(1.0f, glm::dot(glm::normalize(wo), glm::normalize(sr.normal))));
	float etai = 1, etat = mRefractiveIndex;

	math::Vector normal = sr.normal;
	if (cosi < 0.0f) {
		cosi = -cosi;
		normal = -normal;
		std::swap(etai, etat);
	}

	float eta = etai / etat;
	float k = 1 - eta * eta * (1 - cosi * cosi);

	wi = k < 0 ? math::Vector(1, 0, 0) : wo * eta + normal * (eta * cosi - sqrtf(k));

	math::Ray<math::Vector> outgoing_ray{};
	outgoing_ray.d = wi;
	// get the starting position
	math::Point hit = sr.ray.o + sr.t * sr.ray.d;
	outgoing_ray.o = glm::dot(wi, normal) < 0.0f ? hit - (1e-3f * sr.normal) : hit + (1e-3f * sr.normal);

	L += traceRay(sr.world, outgoing_ray);

	return L;
}

#pragma endregion

#pragma region GlossyReflection

GlossyReflection::GlossyReflection(Phong base) : Phong{base}, mGlossyReflectionBRDF{}
{
}

Colour GlossyReflection::shade(ShadeRec& sr) {
	Colour L{ 0,0,0 };//{ Phong::shade(sr) };

	math::Vector wo = -sr.ray.d;

	// Calculates the reflection
	float n_dot_in = glm::dot(glm::normalize(sr.normal), glm::normalize(wo));
	math::Vector wi{ glm::normalize(-wo + 2.0f * sr.normal * n_dot_in) };

	
	L += mGlossyReflectionBRDF.fn(sr, wo, wi);

	return L;
}

#pragma endregion

#pragma endregion

#pragma region Lights

#pragma region Base

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

#pragma endregion

#pragma region Directional

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

#pragma endregion

#pragma region Ambient

Ambient::Ambient() : Light{}
{}

atlas::math::Vector Ambient::getDirection([[maybe_unused]] ShadeRec& sr)
{
	return atlas::math::Vector{ 0.0f };
}

#pragma endregion

#pragma region Point
PointLight::PointLight(math::Point position) : mPosition{ position }, Light{}
{

}

atlas::math::Vector PointLight::getDirection(ShadeRec& sr)
{
	atlas::math::Vector point = sr.ray.d * sr.t + sr.ray.o;
	return glm::normalize(mPosition - point);
}
#pragma endregion

#pragma endregion

#pragma region Camera

#pragma region Base

Camera::Camera() :
	mEye{ 0.0f, 0.0f, 500.0f },
	mLookAt{ 0.0f },
	mUp{ 0.0f, 1.0f, 0.0f },
	mU{ 1.0f, 0.0f, 0.0f },
	mV{ 0.0f, 1.0f, 0.0f },
	mW{ 0.0f, 0.0f, -1.0f }
{
}

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
	mW = glm::normalize(mLookAt - mEye);
	mU = glm::normalize(glm::cross(mUp, mW));
	mV = glm::cross(mW, mU);

	if (atlas::core::areEqual(mEye.x, mLookAt.x) && atlas::core::areEqual(mEye.z, mLookAt.z) &&
		mEye.y > mLookAt.y)
	{
		mU = { 0.0f, 0.0f, 1.0f };
		mV = { 1.0f, 0.0f, 0.0f };
		mW = { 0.0f, 1.0f, 0.0f };
	}

	if (atlas::core::areEqual(mEye.x, mLookAt.x) && atlas::core::areEqual(mEye.z, mLookAt.z) &&
		mEye.y < mLookAt.y)
	{
		mU = { 1.0f, 0.0f, 0.0f };
		mV = { 0.0f, 0.0f, 1.0f };
		mW = { 0.0f, -1.0f, 0.0f };
	}
}

#pragma endregion

#pragma region PinHole

PinHoleCamera::PinHoleCamera(float distEyeToScreen) : mDistEyeToScreen{ distEyeToScreen }, Camera{}
{
}

math::Vector PinHoleCamera::rayDirection(math::Point point) const {
	return glm::normalize(mW * mDistEyeToScreen + mU * point.x + mV * point.y);
}

void PinHoleCamera::renderSection(std::shared_ptr<World>& world, int x_0, int y_0, int x_1, int y_1) const&
{
	using atlas::math::Point;
	using atlas::math::Ray;
	using atlas::math::Vector;

	Point samplePoint{}, pixelPoint{};
	Ray<atlas::math::Vector> ray{};

	ray.o = mEye;
	float avg{ 1.0f / world->sampler->getNumSamples() };

	for (int r{ y_0 }; r < y_1; ++r)
	{
		for (int c{ x_0 }; c < x_1; ++c)
		{
			Colour pixelAverage{ 0, 0, 0 };

			for (int j = 0; j < world->sampler->getNumSamples(); ++j)
			{
				samplePoint = world->sampler->sampleUnitSquare();
				pixelPoint.x = c - 0.5f * world->width + samplePoint.x;
				pixelPoint.y = r - 0.5f * world->height + samplePoint.y;
				ray.d = rayDirection(pixelPoint);

				pixelAverage += traceRay(world, ray);
			}

			world->image[c + (r * world->width)] = Colour{ pixelAverage.r * avg,
								   pixelAverage.g * avg,
								   pixelAverage.b * avg };
		}
	}
}

void PinHoleCamera::renderScene(std::shared_ptr<World>& world) const
{
	world->image.resize(world->width * world->height);

	std::vector<std::thread> threads{};

	auto num_threads = std::thread::hardware_concurrency();
	auto y_section_length = world->height / num_threads;

	for (int i{ 0 }; i < num_threads; ++i) {
		int begin_y = i * y_section_length;
		int end_y = ((i + 1) * y_section_length);

		threads.push_back(std::thread(&PinHoleCamera::renderSection, this, world, 0, begin_y, world->width, end_y));
	}

	for (auto& t : threads) {
		t.join();
	}

	// Out of gamut color handling
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
}

#pragma endregion

#pragma endregion

#pragma region Shapes

#pragma region Base

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

#pragma endregion

#pragma region Sphere

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
		sr.normal = glm::normalize((ray.o + t * ray.d) - mCentre);
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

#pragma endregion

#pragma region Triangle

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

#pragma endregion

#pragma region Plane

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

#pragma endregion

#pragma region Mesh
Mesh::Mesh(std::string filename, math::Point position, float scale) : mPosition{ position } {
	// read in the file using tinyobj
	// create the triangles and add them to the mesh
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn;
	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());

	if (!warn.empty()) {
		std::cout << warn << std::endl;
	}

	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	if (!ret) {
		exit(1);
	}

	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			// Loop over vertices in the face.
			std::vector<math::Point> points{};
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];

				points.push_back((math::Point{ vx, vy, vz } *scale) + mPosition);
			}

			mTriangles.push_back(Triangle{ points[0], points[1], points[2] });

			index_offset += fv;

			// per-face material
			shapes[s].mesh.material_ids[f];
		}
	}
}

bool Mesh::hit(atlas::math::Ray<atlas::math::Vector> const& ray,
	ShadeRec& sr) const {
	bool hit{ false };

	for (auto& tri : mTriangles) {
		hit |= tri.hit(ray, sr);
	}

	if (hit) {
		sr.material = mMaterial;
	}

	return hit;
}

bool Mesh::intersectRay(atlas::math::Ray<atlas::math::Vector> const& ray,
	float& tMin) const
{
	return false;
}

#pragma endregion

#pragma endregion

