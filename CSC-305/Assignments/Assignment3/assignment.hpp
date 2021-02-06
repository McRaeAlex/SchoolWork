#pragma once

#include "paths.hpp"

#include <exception>
#include <iostream>
#include <string>

#include <atlas/glx/Buffer.hpp>
#include <atlas/glx/Context.hpp>
#include <atlas/glx/ErrorCallback.hpp>
#include <atlas/glx/GLSL.hpp>

#include <atlas/math/Math.hpp>

#include <fmt/printf.h>
#include <magic_enum.hpp>

using namespace atlas;

static const std::vector<std::string> IncludeDir{ ShaderPath };

struct OpenGLError : std::runtime_error
{
	OpenGLError(const std::string& what_arg) : std::runtime_error(what_arg) {};
	OpenGLError(const char* what_arg) : std::runtime_error(what_arg) {};
};

// SHAPES

class Cube {
public:
	// Create the cube data
	Cube(math::Point position, float scale, math::Matrix4 rotation);

	// Free the resources the cube uses
	~Cube();

	// Render renders the cube
	void Render(math::Matrix4 view, math::Matrix4 projection, math::Point camera_position, math::Point light_position);
private:
	// clang-format off
	const std::vector<float> mVertices = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	// mGeometry is the vertices of the cube
	const std::vector<float> mGeometry = {
		// The cube without any modifications goes from -1 to 1 on all axis
		// These coordinates are around the origin, so imagine your standing at the origin and placing these points around you
		1.0f, 1.0f, 1.0f, // right, top, back 0
		1.0f, 1.0f, -1.0f, // right, top, front 1
		1.0f, -1.0f, 1.0f, // right, bottom, back 2
		1.0f, -1.0f, -1.0f, // right, bottom, front 3
	   -1.0f, 1.0f, 1.0f, // left, top, back 4
	   -1.0f, 1.0f, -1.0f, // left, top, front 5
	   -1.0f, -1.0f, 1.0f, // left, bottom, back 6 
	   -1.0f, -1.0f, -1.0f, // left, bottom, front 7
	};


	// mIndices are the indices into the mGeometry vertices
	const std::vector<unsigned int> mIndices = {
		// front 0 2 4 6
		0, 4, 6,
		6, 2, 0,

		// right 0 1 2 3
		0, 2, 3,
		3, 1, 0,

		// back 1 3 5 7
		1, 3, 7,
		7, 5, 1,

		// left 4 5 6 7
		4, 5, 7,
		7, 6, 4,

		// top 0 1 4 5
		1, 5, 4,
		4, 0, 1,

		// bottom 2 3 6 7
		2, 6, 7,
		7, 3, 2,
	};
	// clang-format on

	 // mPosition is the position of the cube in world coordinates
	math::Point mPosition;

	// mScale is the size of the cube.
	float mScale;

	// mRotation is the rotation of the cube
	math::Matrix4 mRotation;

	// mProgram is the program used to render a cube.
	GLuint mProgram;

	// mBuffer is the buffer that represents the cube on the gpu
	GLuint mBuffer;

	// mIndexBuffer is the indices for the vertices to create a cube
	// GLuint mIndexBuffer;

	// mVao is the vao object for this cube
	GLuint mVao;

	// mModelLoc, mViewLoc, mProjLoc, mLightLoc, mCameraLoc are all location of uniform buffers that get initalized on cube creation.
	GLuint mModelLoc, mViewLoc, mProjLoc, mLightLoc, mCameraLoc;
};

class PointLight {
public:
	// maybe add intensity and stuff?
	PointLight(math::Point position);

	// getPosition
	glm::vec3 getPosition();

private:
	// mPosition is the position of the light
	math::Point mPosition;
};



class PinHoleCamera {
	/*
	 * The pinhole camera needs a constructor which defaults to pointing down the negative z
	 * We then need a way to orient the camera every frame. We can do this after.
	 */

public:
	PinHoleCamera(math::Point eye, math::Point lookAt, float fov);

	// getViewMatrix returns the matrix that represents the camera.
	math::Matrix4 getViewMatrix();
	math::Matrix4 getProjectionMatrix(int width, int height);
	glm::vec3 getPosition();
	void handleKeyboardEvent(GLFWwindow* window);
	void handleMouseEvent(float delta_x, float delta_y);

private:
	// mPosition is the current position of the camera. Sometimes called eye
	math::Point mPosition;

	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
	glm::vec3 cameraRight;

	float yaw = -90.0f;
	float pitch = 0.0f;

	// mLookAt is the point the camera is looking at. This point needs to be generated for a bunch of other things. Like yaw, pitch, roll etc. Or we can use Quaternions.
	math::Point mLookAt;

	// mFov is the field of view for the camera
	float mFov;

	void updateVectors();
};

// Program
class Program
{
public:
	Program(int width, int height, std::string title);

	void run(Cube& cube);

	void freeGPUData();

private:
	static void errorCallback(int code, char const* message)
	{
		fmt::print("error ({}): {}\n", code, message);
	}

	void createGLContext();

	void processInput();
	void processMouse(double xpos, double ypos);

	float lastX;
	float lastY;

	PinHoleCamera mCamera;

	GLFWwindow* mWindow;
	glx::WindowSettings settings;
	glx::WindowCallbacks callbacks;
};