#include "assignment.hpp"
#include <functional>

int main()
{
	try
	{
		// clang-format off
		std::array<float, 18> vertices =
		{
			// Vertices          Colours
			0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,
		   -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,
			0.0f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f
		};
		// clang-format on

		Program prog{ 1280, 720, "CSC305 Lab 5" };
		Cube cube{ math::Vector(0.0, 0.0, -10.0), 1.0f, glm::rotate(glm::mat4(1.0f), 0.0f,  glm::vec3(0.5f, 1.0f, 0.0f)) };

		prog.run(cube);

		prog.freeGPUData();
	}
	catch (OpenGLError & err)
	{
		fmt::print("OpenGL Error:\n\t{}\n", err.what());
	}

	return 0;
}

// Cube
// --------------

Cube::Cube(math::Point position, float scale, math::Matrix4 rotation) : mPosition{ position }, mScale{ scale }, mRotation{ rotation }
{
	// Create the VAO
	glCreateVertexArrays(1, &mVao);

	// Create a buffer for it. Ideally this can be static because the geometry for a cube stays the same but for now we will have one buffer per cube.
	glCreateBuffers(1, &mBuffer);
	glNamedBufferStorage(mBuffer, sizeof(float)* mVertices.size(), mVertices.data(), 0);

	// Attach the buffer to the vao. Setting its data format and 
	glVertexArrayVertexBuffer(mVao, 0, mBuffer, 0, glx::stride<float>(6));

	glVertexArrayAttribBinding(mVao, 0, 0);
	glVertexArrayAttribFormat(mVao, 0, 3, GL_FLOAT, GL_FALSE, 0);
	glEnableVertexArrayAttrib(mVao, 0);

	glVertexArrayAttribBinding(mVao, 1, 0);
	glVertexArrayAttribFormat(mVao, 1, 3, GL_FLOAT, GL_FALSE, glx::relativeOffset<float>(3));
	glEnableVertexArrayAttrib(mVao, 1);

	// Note we cannot bind a IBO to a VAO

	// Lets compile a shader
	std::string shaderPath(ShaderPath);

	auto vertexSource = glx::readShaderSource(shaderPath + "basic.vert", IncludeDir);
	auto fragmentSource = glx::readShaderSource(shaderPath + "basic.frag", IncludeDir);

	GLuint vertexHandle = glCreateShader(GL_VERTEX_SHADER);
	if (auto result = glx::compileShader(vertexSource.sourceString, vertexHandle); result) {
		throw std::runtime_error("Failed to compilethe vertex shader");
	}

	GLuint fragmentHandle = glCreateShader(GL_FRAGMENT_SHADER);
	if (auto result = glx::compileShader(fragmentSource.sourceString, fragmentHandle); result) {
		throw std::runtime_error("Failed to compilethe fragment shader");
	}

	mProgram = glCreateProgram();
	glAttachShader(mProgram, vertexHandle);
	glAttachShader(mProgram, fragmentHandle);
	if (auto result = glx::linkShaders(mProgram); result) {
		throw std::runtime_error("Failed to link the program");
	}

	glDetachShader(mProgram, vertexHandle);
	glDetachShader(mProgram, fragmentHandle);
	glDeleteShader(vertexHandle);
	glDeleteShader(fragmentHandle);

	mModelLoc = glGetUniformLocation(mProgram, "model");
	mViewLoc = glGetUniformLocation(mProgram, "view");
	mProjLoc = glGetUniformLocation(mProgram, "projection");
	mLightLoc = glGetUniformLocation(mProgram, "lightPosition");
	mCameraLoc = glGetUniformLocation(mProgram, "cameraPosition");
}

Cube::~Cube() {
	// Free GPU resources
	glDeleteProgram(mProgram);
	glDeleteVertexArrays(1, &mVao);
	glDeleteBuffers(1, &mBuffer);
}

void Cube::Render(math::Matrix4 view, math::Matrix4 projection, math::Point camera_position, math::Point light_position) {
	// Use the program to draw the cube
	glUseProgram(mProgram);

	// Bind the data
	glBindVertexArray(mVao);

	// Prep the matrices

	math::Matrix4 model = glm::mat4(1.0f);

	model = glm::translate(model, mPosition);
	model = glm::rotate(model, (float)glfwGetTime() * glm::radians(55.0f), glm::vec3(0.5f, 1.0f, 0.0f)); // This is just for fun it can be changed later
	model = glm::scale(model, glm::vec3(mScale));

	// Set the uniforms


	glProgramUniformMatrix4fv(mProgram, mModelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glProgramUniformMatrix4fv(mProgram, mViewLoc, 1, GL_FALSE, glm::value_ptr(view));

	glProgramUniformMatrix4fv(mProgram, mProjLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glProgramUniform3fv(mProgram, mLightLoc, 1, glm::value_ptr(light_position));

	glProgramUniform3fv(mProgram, mCameraLoc, 1, glm::value_ptr(camera_position));

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
}

// ===------------IMPLEMENTATIONS-------------===

Program::Program(int width, int height, std::string title) :
	settings{}, callbacks{}, mWindow{ nullptr }, mCamera{ math::Vector(0.0, 0.0, 0.0), math::Vector(0.0, 0.0, -1.0f), glm::radians(45.0f) }
{
	settings.size.width = width;
	settings.size.height = height;
	settings.title = title;

	lastX = width / 2.0f;
	lastY = height / 2.0f;

	callbacks.mouseMoveCallback = [this](double a, double b) {
		this->processMouse(a, b);
	};

	if (!glx::initializeGLFW(errorCallback))
	{
		throw OpenGLError("Failed to initialize GLFW with error callback");
	}

	mWindow = glx::createGLFWWindow(settings);
	if (mWindow == nullptr)
	{
		throw OpenGLError("Failed to create GLFW Window");
	}


	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	createGLContext();
}

void Program::run(Cube& cube)
{
	// Do basic init
	glEnable(GL_DEPTH_TEST);
	PointLight light{ math::Vector(0.0f, 1.0f, -2.0f) };
	while (!glfwWindowShouldClose(mWindow))
	{
		int width;
		int height;
		glfwGetFramebufferSize(mWindow, &width, &height);
		// setup the view to be the window's size
		glViewport(0, 0, width, height);
		// tell OpenGL the what color to clear the screen to
		glClearColor(0, 0, 0, 1);
		// actually clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput();

		math::Matrix4 view = mCamera.getViewMatrix();
		math::Matrix4 projection = mCamera.getProjectionMatrix(width, height);

		cube.Render(view, projection, mCamera.getPosition(), light.getPosition());

		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}
}

void Program::processInput() {
	mCamera.handleKeyboardEvent(mWindow);
}

void Program::processMouse(double xpos, double ypos) {
	float xoffset = (float)xpos - lastX;
	float yoffset = lastY - (float)ypos;

	lastX = (float)xpos;
	lastY = (float)ypos;

	mCamera.handleMouseEvent(xoffset, yoffset);
}

void Program::freeGPUData()
{
	glx::destroyGLFWWindow(mWindow);
	glx::terminateGLFW();
}

void Program::createGLContext()
{
	glx::bindWindowCallbacks(mWindow, callbacks);
	glfwMakeContextCurrent(mWindow);
	glfwSwapInterval(1);

	if (!glx::createGLContext(mWindow, settings.version))
	{
		throw OpenGLError("Failed to create OpenGL context");
	}

	glx::initializeGLCallback(
		glx::ErrorSource::All, glx::ErrorType::All, glx::ErrorSeverity::All);
}


// PinHoleCamera
// ---------------------------------

PinHoleCamera::PinHoleCamera(math::Point eye, math::Point lookAt, float fov) : mPosition{ eye }, mLookAt{ lookAt }, mFov{ fov }
{
	updateVectors();
}

math::Matrix4 PinHoleCamera::getViewMatrix() {
	return glm::lookAt(mPosition, mPosition + cameraFront, cameraUp);
}

math::Matrix4 PinHoleCamera::getProjectionMatrix(int width, int height) {
	return glm::perspective(mFov, (float)width / height, 0.1f, 100.0f);
}

glm::vec3 PinHoleCamera::getPosition() {
	return mPosition;
}

void PinHoleCamera::handleKeyboardEvent(GLFWwindow* window) {
	const float cameraSpeed = 0.02f; // adjust accordingly
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		mPosition += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		mPosition -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		mPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		mPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void PinHoleCamera::handleMouseEvent(float delta_x, float delta_y) {
	delta_x *= 0.1f;
	delta_y *= 0.1f;

	yaw += delta_x;
	pitch += delta_y;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;


	// Update Front, Right and Up Vectors using the updated Euler angles
	updateVectors();
}

void PinHoleCamera::updateVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

	cameraRight = glm::normalize(glm::cross(cameraFront, glm::vec3(0.0f, 1.0f, 0.0f)));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

// PointLight

PointLight::PointLight(math::Point position) : mPosition{ position } {
}

glm::vec3 PointLight::getPosition() {
	return mPosition;
}