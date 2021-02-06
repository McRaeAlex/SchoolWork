#pragma once

#include <atlas/core/Float.hpp>
#include <atlas/math/Math.hpp>
#include <atlas/math/Random.hpp>
#include <atlas/math/Ray.hpp>

#include <fmt/printf.h>
#include <stb_image.h>
#include <stb_image_write.h>

#include <limits>
#include <memory>
#include <vector>
#include <string>

using namespace atlas;
using Colour = math::Vector;


void saveToBMP(std::string const& filename,
               std::size_t width,
               std::size_t height,
               std::vector<Colour> const& image);
// Declarations
class BRDF;
class Camera;
class Material;
class Light;
class Shape;
class Sampler;

struct World
{
    std::size_t width, height;
    Colour background;
    std::shared_ptr<Sampler> sampler;
    std::vector<std::shared_ptr<Shape>> scene;
    std::vector<Colour> image;
    std::vector<std::shared_ptr<Light>> lights;
    std::shared_ptr<Light> ambient;
};

struct ShadeRec
{
    Colour color;
    float t;
    atlas::math::Normal normal;
    atlas::math::Ray<atlas::math::Vector> ray;
    std::shared_ptr<Material> material;
    std::shared_ptr<World> world;
};

#pragma region Cameras

class Camera
{
public:
    Camera();

    virtual ~Camera() = default;

    virtual void renderScene(std::shared_ptr<World>& world) const = 0;

    void setEye(atlas::math::Point const& eye);

    void setLookAt(atlas::math::Point const& lookAt);

    void setUpVector(atlas::math::Vector const& up);

    void computeUVW();

protected:
    atlas::math::Point mEye;
    atlas::math::Point mLookAt;
    atlas::math::Point mUp;
    atlas::math::Vector mU, mV, mW;
};

class PinHoleCamera : public Camera {
public:
    PinHoleCamera(float distEyeToScreen);

    void renderSection(std::shared_ptr<World>& world, int x_0, int y_0, int x_1, int y_1) const&;
    void renderScene(std::shared_ptr<World>& world) const;
    math::Vector rayDirection(math::Point point) const;

protected:
    float mDistEyeToScreen;
};

#pragma endregion

#pragma region Samplers

class Sampler
{
public:
    Sampler(int numSamples, int numSets);
    virtual ~Sampler() = default;

    int getNumSamples() const;

    void setupShuffledIndeces();

    virtual void generateSamples() = 0;

    atlas::math::Point sampleUnitSquare();

protected:
    std::vector<atlas::math::Point> mSamples;
    std::vector<int> mShuffledIndeces;

    int mNumSamples;
    int mNumSets;
    unsigned long mCount;
    int mJump;
};

class NRooksSampler : public Sampler {
public:
    NRooksSampler(int numSamples, int numSets);

    void generateSamples();
};

#pragma endregion

#pragma region BRDFs

class BRDF
{
public:
    virtual ~BRDF() = default;

    virtual Colour fn(ShadeRec const& sr,
        atlas::math::Vector const& reflected,
        atlas::math::Vector const& incoming) const = 0;
    virtual Colour rho(ShadeRec const& sr,
        atlas::math::Vector const& reflected) const = 0;
};


class Lambertian : public BRDF
{
public:
    Lambertian();
    Lambertian(Colour diffuseColor, float diffuseReflection);

    Colour fn(ShadeRec const& sr,
        atlas::math::Vector const& reflected,
        atlas::math::Vector const& incoming) const override;

    Colour rho(ShadeRec const& sr,
        atlas::math::Vector const& reflected) const override;

    void setDiffuseReflection(float kd);

    void setDiffuseColour(Colour const& colour);

private:
    Colour mDiffuseColour;
    float mDiffuseReflection;
};

class Specular : public BRDF {
public:
    Specular();
    Specular(float intensity, Colour color);

    Colour fn(ShadeRec const& sr,
        atlas::math::Vector const& reflected,
        atlas::math::Vector const& incoming) const override;

    Colour rho(ShadeRec const& sr,
        atlas::math::Vector const& reflected) const override;

    void setDiffuseColour(Colour color);
    void setSpecularReflection(float ks);

private:
    Colour mSpecularColour;
    float mSpecularReflection;
};

class GlossySpecular : public BRDF {
public:
    GlossySpecular();
    ~GlossySpecular();

    Colour fn(ShadeRec const& sr,
        atlas::math::Vector const& reflected,
        atlas::math::Vector const& incoming) const;
    Colour rho(ShadeRec const& sr,
        atlas::math::Vector const& reflected) const;
};

#pragma endregion

#pragma region Materials

class Material
{
public:
    virtual ~Material() = default;

    virtual Colour shade(ShadeRec& sr) = 0;
};

class Matte : public Material
{
public:
    Matte();
    Matte(float kd, float ka, Colour color);

    void setDiffuseReflection(float k);

    void setAmbientReflection(float k);

    void setDiffuseColour(Colour colour);

    Colour shade(ShadeRec& sr) override;

private:
    std::shared_ptr<Lambertian> mDiffuseBRDF;
    std::shared_ptr<Lambertian> mAmbientBRDF;
};

class Phong : public Material
{
public:
    Phong(float kd, float ka, float ks, Colour color, Colour specularColor);
    Phong(const Phong& copy);

    Colour shade(ShadeRec& sr) override;

private:
    std::shared_ptr<Lambertian> mDiffuseBRDF;
    std::shared_ptr<Lambertian> mAmbientBRDF;
    std::shared_ptr<Specular> mSpecularBRDF;
};

class SimpleTransparent : public Phong {
public:
    SimpleTransparent(Phong base, float ref);

    Colour shade(ShadeRec& sr) override;

private:
    float mRefractiveIndex;
};

class GlossyReflection : public Phong {
public:
    GlossyReflection(Phong base);

    Colour shade(ShadeRec& sr) override;

private:
    GlossySpecular mGlossyReflectionBRDF;
};

#pragma endregion

#pragma region Lights

class Light
{
public:
    virtual atlas::math::Vector getDirection(ShadeRec& sr) = 0;

    virtual Colour L(ShadeRec& sr);

    void scaleRadiance(float b);

    void setColour(Colour const& c);

protected:
    Colour mColour;
    float mRadiance;
};

class Directional : public Light
{
public:
    Directional();
    Directional(atlas::math::Vector const& d);

    void setDirection(atlas::math::Vector const& d);

    atlas::math::Vector getDirection(ShadeRec& sr) override;

private:
    atlas::math::Vector mDirection;
};

class Ambient : public Light
{
public:
    Ambient();

    atlas::math::Vector getDirection(ShadeRec& sr) override;
private:
    atlas::math::Vector mDirection;
};

class PointLight : public Light
{
public:
    PointLight(math::Point position);

    atlas::math::Vector getDirection(ShadeRec& sr) override;

private:
    math::Point mPosition;
};
#pragma endregion

#pragma region Shapes

class Shape
{
public:
    Shape();
    virtual ~Shape() = default;

    // if t computed is less than the t in sr, it and the color should be
    // updated in sr
    virtual bool hit(atlas::math::Ray<atlas::math::Vector> const& ray,
        ShadeRec& sr) const = 0;

    void setColour(Colour const& col);

    Colour getColour() const;

    void setMaterial(std::shared_ptr<Material> const& material);

    std::shared_ptr<Material> getMaterial() const;

protected:
    virtual bool intersectRay(atlas::math::Ray<atlas::math::Vector> const& ray,
        float& tMin) const = 0;

    Colour mColour;
    std::shared_ptr<Material> mMaterial;
};


class Sphere : public Shape
{
public:
    Sphere(atlas::math::Point center, float radius);

    bool hit(atlas::math::Ray<atlas::math::Vector> const& ray,
        ShadeRec& sr) const;

private:
    bool intersectRay(atlas::math::Ray<atlas::math::Vector> const& ray,
        float& tMin) const;

    atlas::math::Point mCentre;
    float mRadius;
    float mRadiusSqr;
};

class Plane : public Shape {
public:
    Plane(atlas::math::Point p, atlas::math::Vector n);

    bool hit(atlas::math::Ray<atlas::math::Vector> const& ray,
        ShadeRec& sr) const;

private:
    atlas::math::Vector mNormal;
    atlas::math::Point mPoint;

    bool intersectRay(atlas::math::Ray<atlas::math::Vector> const& ray,
        float& tMin) const;
};

class Triangle : public Shape {
public:
    Triangle(atlas::math::Point v1, atlas::math::Point v2, atlas::math::Point v3);

    bool hit(atlas::math::Ray<atlas::math::Vector> const& ray,
        ShadeRec& sr) const;

private:
    bool intersectRay(atlas::math::Ray<atlas::math::Vector> const& ray,
        float& tMin) const;

    atlas::math::Vector mNormal;
    atlas::math::Point mV1, mV2, mV3;
};

class Mesh : public Shape {
public:
    Mesh(std::string filename, math::Point position, float scale);

    bool hit(atlas::math::Ray<atlas::math::Vector> const& ray,
        ShadeRec& sr) const;

private:
    bool intersectRay(atlas::math::Ray<atlas::math::Vector> const& ray,
        float& tMin) const;

    std::vector<Triangle> mTriangles;
    math::Point mPosition;
};
#pragma endregion

